#include "uart.h"
#include "ui_uart.h"
#include "serialport.h"
#include <string>
#include <QDebug>
#include <QMessageBox>
#include <QHBoxLayout>

uart::uart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::uart)
{
    ui->setupUi(this);

    QPixmap close(":/icons/images/icons-close-48.png");
    QIcon closeIcon(close);
    ui->btnExit->setIcon(closeIcon);
    ui->btnExit->setIconSize(close.rect().size());

    QHBoxLayout *framelayout = new QHBoxLayout;
    portsettings = new serialport(this);
    connect(portsettings, SIGNAL(port_update()), this, SLOT(update_port()));
    ui->mainLayout->addWidget(portsettings);

    txtFrame = new QFrame(this);
    txtFrame->setLayout(framelayout);
    ui->mainLayout->addWidget(txtFrame);

    txtBoxRead = new QPlainTextEdit(txtFrame);
    txtBoxRead->setReadOnly(true);
    txtBoxRead->setPlainText("Serial Read Window");
    txtBoxWrite = new QPlainTextEdit(txtFrame);
    txtBoxWrite->setPlainText("Sample data to write on port");

    framelayout->addWidget(txtBoxRead);
    framelayout->addWidget(txtBoxWrite);

    lblstatus = new QLabel(this);
    ui->mainLayout->addWidget(lblstatus);

    serial = new QSerialPort(this);
    serialBuffer = "";
    serialData = "";
    is_available = false;

    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        portList.append(serialPortInfo.portName());
        qDebug() << "Serial Port" << serialPortInfo.portName();
    }

    portsettings->setSerialPortItem(portList);
    update_port();
    serial->setPortName(_serialport);
    serial->setBaudRate(_baud);
    serial->setDataBits(QSerialPort::Data8);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);

    QObject::connect(serial, SIGNAL(readyRead()), this, SLOT(readSerial()));
    QObject::connect(this, SIGNAL(dataRead(QString)), this, SLOT(update_read_ui(QString)));

    lblstatus->setText("Ready to connect");
}

uart::~uart()
{
    serial->close();
    delete ui;
}

void uart::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}
void uart::update_port()
{
    // update uart settings here
    qDebug() << "Updating serial port settings";
    _serialport = portsettings->get_port();
    _baud = portsettings->get_baud();
    _parity = portsettings->get_parity();
    _stopBits = portsettings->get_stopbit();
    _dataBits = portsettings->get_data();
    _rts = 0;
    _timeOut = 1;

    serial->setPortName("/dev/" + _serialport);
    serial->setBaudRate(_baud);
    switch (_dataBits) {
    case 8:
        serial->setDataBits(QSerialPort::Data8);
        break;
    case 7:
        serial->setDataBits(QSerialPort::Data7);
        break;
    default:
        break;
    }
    switch (_stopBits) {
    case 1:
        serial->setStopBits(QSerialPort::OneStop);
        break;
    case 2:
        serial->setStopBits(QSerialPort::TwoStop);
        break;
    default:
        break;
    }
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->setParity(QSerialPort::NoParity);
    lblstatus->setText("Status: Port updated Press open to connect");
}

void uart::on_btnExit_clicked()
{
    serial->close();
    this->close();
}
void uart::readSerial()
{
    while (serial->bytesAvailable())
    {
        serialData.append(serial->readAll());
        if(serialData.contains("\r"))
        {
            serialBuffer += QString::fromStdString(serialData.toStdString());
            qDebug() << serialBuffer;
            emit dataRead(serialBuffer);
            serialData.clear();
            serialBuffer.clear();
            break;
        }
    }
}

void uart::update_read_ui(QString readstring)
{
    txtBoxRead->setPlainText(readstring);
}

void uart::on_btnOpen_toggled(bool checked)
{
    if(checked)
    {
        if(portList.contains(_serialport))
        {
            update_port();
            // open port here
            if(serial->open(QSerialPort::ReadWrite) == true)
            {
                is_available = true;
                lblstatus->setText("Status:Connected");
                ui->btnOpen->setText("Close");
            }
            else
            {
                qDebug() << "error opening port";
                lblstatus->setText("Status:error opening port");
                ui->btnOpen->setChecked(false);
            }
        }else
        {
            qDebug() << "No serial Port " << _serialport;
            lblstatus->setText("Status: No serial Port");
            ui->btnOpen->setChecked(false);
        }

    }
    else
    {
        if(is_available){
            // close port here
            lblstatus->setText("Status:Closed");
            serial->close();
            ui->btnOpen->setText("Open");
        }
    }
}

void uart::on_btnClear_clicked()
{
    txtBoxWrite->clear();
    txtBoxRead->clear();
}

void uart::on_btnSend_clicked()
{
    QString writeString = txtBoxWrite->toPlainText();
    QByteArray writeBytes = writeString.toUtf8();
    qDebug() << "sending: " << writeString ;
    serial->write(writeBytes);
}
