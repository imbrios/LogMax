#include "modbus_form.h"
#include "ui_modbus_form.h"
#include <QTableWidget>
#include <QHBoxLayout>
#include <QFrame>
#include <QSerialPort>
#include <QModbusRtuSerialSlave>
#include <QModbusTcpServer>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QStatusBar>
#include <QUrl>

modbus_form::modbus_form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::modbus_form)
{
    ui->setupUi(this);
    _connected = false;
    QHBoxLayout *frameLayout = new QHBoxLayout;
    QFrame *txtFrame = new QFrame(this);
    txtFrame->setLayout(frameLayout);
    this->setAutoFillBackground(true);
    port = new serialport(this);
    request = new modbusrequest(this);
    txtRead = new QPlainTextEdit(txtFrame);
    txtWrite = new QPlainTextEdit(txtFrame);
    lblStatus = new QLabel(this);
    connect(request, SIGNAL(request_ready(int,int,int,int)), this, SLOT(send_request(int,int,int,int)));
    connect(port, SIGNAL(port_update()), this, SLOT(update_port()));
    ui->mainlayout->addWidget(port);
    ui->mainlayout->addWidget(request);
    frameLayout->addWidget(txtRead);
    frameLayout->addWidget(txtWrite);
    ui->mainlayout->addWidget(txtFrame);
    ui->mainlayout->addWidget(lblStatus);
    lblStatus->setText("Status:");
    ui->btnConnect->setChecked(_connected);
    txtWrite->setEnabled(false);
    txtRead->setReadOnly(true);
    QPixmap close(":/icons/images/icons-close-48.png");
    QIcon closeIcon(close);
    ui->btnExit->setIcon(closeIcon);
    ui->btnExit->setIconSize(close.rect().size());
    update_port();
}

modbus_form::~modbus_form()
{
    delete ui;
}

void modbus_form::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}

void modbus_form::send_request(int slave, int functioncode, int address, int no_of_coils)
{
    //Modbus request data
    if(!_connected){
        qDebug() << "Modbus not connected";
        lblStatus->setText("Status: Modbus not connected");
        return;
    }
    qDebug() <<  "Modbus request to SLave = " << slave <<"Function Code = "<< functioncode;
    QApplication::setOverrideCursor(Qt::WaitCursor);

    switch(functioncode)
    {
    case MODBUS_FC_READ_COILS:
    case MODBUS_FC_READ_DISCRETE_INPUTS:
    case MODBUS_FC_READ_HOLDING_REGISTERS:
    case MODBUS_FC_READ_INPUT_REGISTERS:
        txtWrite->setEnabled(false);
        read(slave, functioncode, address, no_of_coils);
        break;

    case MODBUS_FC_WRITE_SINGLE_COIL:
        txtWrite->setEnabled(true);
        write(slave, address, (uint8_t)txtWrite->toPlainText().toInt());
        break;
    case MODBUS_FC_WRITE_SINGLE_REGISTER:
        txtWrite->setEnabled(true);
        write(slave, address, (uint16_t)txtWrite->toPlainText().toInt());
        break;
    case MODBUS_FC_WRITE_MULTIPLE_COILS:
    case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
        txtWrite->setEnabled(true);
        //write(slave, functioncode, address, no_of_coils);
        break;
    default:
        break;
    }

    QApplication::setOverrideCursor(Qt::ArrowCursor);

}

void modbus_form::update_port()
{
    qDebug() << "Updateing serial port settings";
    _serialport = port->get_port();
    _baud = port->get_baud();
    _parity = port->get_parity();
    _stopBits = port->get_stopbit();
    _dataBits = port->get_data();
    _timeOut = 1;
}
void modbus_form::begin(QString port, int baud, char parity, int databit, int stopbit)
{
    qDebug() << "Modbus Lib version " <<  LIBMODBUS_VERSION_STRING;
    qDebug() << "Modbus Connecting to =>"<< port << baud <<  parity <<  databit <<  stopbit;
    _modbus = modbus_new_rtu(port.toLatin1().constData(), baud, parity, databit, stopbit);
    modbus_set_debug(_modbus, TRUE);
    if(_modbus == NULL)
    {
        qDebug() << "Error opening modbus port";
        return;
    }
    if(_modbus && modbus_connect(_modbus) == -1)
    {
        modbus_free(_modbus);
        qDebug() << "Error connecting modbus";
        return;
    }
    //modbus_rtu_set_serial_mode(_modbus, MODBUS_RTU_RS232);
    //modbus_set_response_timeout(_modbus, 0, 50000000);
    _connected = true;

    /* Allocate and initialize the different memory spaces */
    int nb = 99 - 0;
    tab_rq_bits = (uint8_t *) malloc(nb * sizeof(uint8_t));
    memset(tab_rq_bits, 0, nb * sizeof(uint8_t));

    tab_rp_bits = (uint8_t *) malloc(nb * sizeof(uint8_t));
    memset(tab_rp_bits, 0, nb * sizeof(uint8_t));

    tab_rq_registers = (uint16_t *) malloc(nb * sizeof(uint16_t));
    memset(tab_rq_registers, 0, nb * sizeof(uint16_t));

    tab_rp_registers = (uint16_t *) malloc(nb * sizeof(uint16_t));
    memset(tab_rp_registers, 0, nb * sizeof(uint16_t));

    tab_rw_rq_registers = (uint16_t *) malloc(nb * sizeof(uint16_t));
    memset(tab_rw_rq_registers, 0, nb * sizeof(uint16_t));
}

void modbus_form::end()
{
    free(tab_rq_bits);
    free(tab_rp_bits);
    free(tab_rq_registers);
    free(tab_rp_registers);
    free(tab_rw_rq_registers);
    if(_modbus) {
        if (_connected){
            modbus_close(_modbus);
            modbus_free(_modbus);
            qDebug() << "Modbus disconnected";
        }
        _modbus = NULL;
    }
    _connected = false;
}
void modbus_form::read(int slave, int functionCode, int startAddress, int noOfItems)
{
    int ret = -1; //return value from read functions
    qDebug() <<  "Modbus Read Function ";
    if(_modbus == NULL)
    {
        qDebug() <<  "Invalid modbus context ";
        return;
    }
    if(modbus_set_slave(_modbus, slave) == -1)
    {
        qDebug() << "Error setting slave";
        return;
    }
    //request data from modbus
    switch(functionCode)
    {
    case MODBUS_FC_READ_COILS:
        ret = modbus_read_bits(_modbus, startAddress, noOfItems, tab_rp_bits);
        break;
    case MODBUS_FC_READ_DISCRETE_INPUTS:
        ret = modbus_read_input_bits(_modbus, startAddress, noOfItems, tab_rq_bits);
        break;
    case MODBUS_FC_READ_HOLDING_REGISTERS:
        ret = modbus_read_registers(_modbus, startAddress, noOfItems, tab_rp_registers);
        break;
    case MODBUS_FC_READ_INPUT_REGISTERS:
        ret = modbus_read_input_registers(_modbus, startAddress, noOfItems, tab_rq_registers);
        break;
    default:
        break;
    }
    qDebug() <<  "Modbus Read Data return value = " << ret << ", errno = " << modbus_strerror(errno);
    if(ret == noOfItems)
    {
        qDebug() << "Data success";
    }
    else
    {
        modbus_flush(_modbus); //flush data
    }
}
// write single coil at address on modbus
void modbus_form::write(int slave, int startAddress, uint8_t coilStatus)
{
    int ret = -1; //return value from functions
    qDebug() <<  "Modbus Write Data Coil";
    if(_modbus == NULL) return;
    modbus_set_slave(_modbus, slave);
    //request data from modbus
    ret = modbus_write_bit(_modbus, startAddress, coilStatus);
    qDebug() <<  "Modbus Write Data return value = " << ret << ", errno = " << errno;;
    //update data model
    if(ret == 1)
    {
        //values written correctly
        qDebug() <<  "Coil value written correctly";
    }
    else
    {
        modbus_flush(_modbus); //flush data
    }

}
// Write single register at mentioned address
void modbus_form::write(int slave, int startAddress, uint16_t registerValue)
{
    qDebug() <<  "Modbus Write Data Coil";
    if(_modbus == NULL) return;
    int ret = -1; //return value from functions
    modbus_set_slave(_modbus, slave);
    //request data from modbus
    ret = modbus_write_register(_modbus, startAddress, registerValue);
    qDebug() <<  "Modbus Write Data return value = " << ret << ", errno = " << errno;;
    //update data model
    if(ret == 1)
    {
        //values written correctly
        qDebug() <<  "register value written correctly";
    }
    else
    {
        modbus_flush(_modbus); //flush data
    }

}

// Write multiple register at mentioned address
void modbus_form::write(int slave, int startAddress, int noOfItems, uint16_t *data)
{
    qDebug() <<  "Modbus Write Data Coil";
    if(_modbus == NULL) return;
    int ret = -1; //return value from functions
    modbus_set_slave(_modbus, slave);
    //request data from modbus
    ret = modbus_write_registers(_modbus, startAddress, noOfItems, data);
    qDebug() <<  "Modbus Write Data return value = " << ret << ", errno = " << errno;;
    //update data model
    if(ret == noOfItems)
    {
        //values written correctly
        qDebug() <<  "register value written correctly";
    }
    else
    {
        modbus_flush(_modbus); //flush data
    }
}

// Write multiple coils at mentioned address
void modbus_form::write(int slave, int startAddress, int noOfItems, uint8_t *data)
{
    qDebug() <<  "Modbus Write Data Coil";
    if(_modbus == NULL) return;
    int ret = -1; //return value from functions
    modbus_set_slave(_modbus, slave);
    //request data from modbus
    ret = modbus_write_bits(_modbus, startAddress, noOfItems, data);
    qDebug() <<  "Modbus Write Data return value = " << ret << ", errno = " << errno;;
    //update data model
    if(ret == noOfItems)
    {
        //values written correctly
        qDebug() <<  "register value written correctly";
    }
    else
    {
        modbus_flush(_modbus); //flush data
    }
}
void modbus_form::on_btnConnect_toggled(bool checked)
{
    if(checked){
        if(!_connected){
            begin(_serialport, _baud, _parity.toLatin1(), _dataBits, _stopBits);
            if(_connected){
                ui->btnConnect->setText("Disconnect");
                lblStatus->setText("Connected");
            }
            else
            {
                ui->btnConnect->setChecked(false);
                lblStatus->setText("Status: Error Opening Port");
            }
        }
    }
    else{
        if(_connected){
            ui->btnConnect->setText("Connect");
            lblStatus->setText("Status: Closed");
            end();
        }
    }
}

void modbus_form::on_btnExec_clicked()
{

}

void modbus_form::on_btnExit_clicked()
{
    end();
    this->close();
    //emit quit();
}
