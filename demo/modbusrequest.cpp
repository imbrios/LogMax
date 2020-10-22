#include "modbusrequest.h"
#include "ui_modbusrequest.h"

/* Modbus function codes */
#define MODBUS_FC_READ_COILS                0x01
#define MODBUS_FC_READ_DISCRETE_INPUTS      0x02
#define MODBUS_FC_READ_HOLDING_REGISTERS    0x03
#define MODBUS_FC_READ_INPUT_REGISTERS      0x04
#define MODBUS_FC_WRITE_SINGLE_COIL         0x05
#define MODBUS_FC_WRITE_SINGLE_REGISTER     0x06
#define MODBUS_FC_READ_EXCEPTION_STATUS     0x07
#define MODBUS_FC_WRITE_MULTIPLE_COILS      0x0F
#define MODBUS_FC_WRITE_MULTIPLE_REGISTERS  0x10
#define MODBUS_FC_REPORT_SLAVE_ID           0x11
#define MODBUS_FC_MASK_WRITE_REGISTER       0x16
#define MODBUS_FC_WRITE_AND_READ_REGISTERS  0x17

#define READ_COIL "Read Coils (0x01)"
#define READ_DISCRETE "Read Discrete Inputs (0x02)"
#define READ_HOLDING_REGISTER "Read Holding Registers (0x03)"
#define READ_INPUT_REGISTER "Read Input Registers (0x04)"
#define WRITE_COIL "Write Single Coil (0x05)"
#define WRITE_REGISTER "Write Single Register (0x06)"
#define WRITE_MULTIPLE_COIL "Write Multiple Coils (0x0f)"
#define WRITE_MULTIPLE_REGISTER "Write Multiple Registers (0x10)"

modbusrequest::modbusrequest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::modbusrequest)
{
    ui->setupUi(this);
    QPixmap close(":/icons/images/icons-forward-arrow-40.png");
    QIcon closeIcon(close);
    ui->pushButton->setIcon(closeIcon);
    ui->pushButton->setIconSize(close.rect().size());

}

modbusrequest::~modbusrequest()
{
    delete ui;
}

void modbusrequest::on_spnSlave_valueChanged(int arg1)
{
    _slave = arg1;
}

void modbusrequest::on_cmbFunctionCode_currentIndexChanged(const QString &arg1)
{
    if(arg1 == READ_COIL){
        _functionCode = MODBUS_FC_READ_COILS;
        return;
    }
    if(arg1 == READ_DISCRETE){
        _functionCode = MODBUS_FC_READ_DISCRETE_INPUTS;
        return;
    }
    if(arg1 == READ_HOLDING_REGISTER){
        _functionCode = MODBUS_FC_READ_HOLDING_REGISTERS;
        return;
    }
    if(arg1 == READ_INPUT_REGISTER){
        _functionCode = MODBUS_FC_READ_INPUT_REGISTERS;
        return;
    }
    if(arg1 == WRITE_COIL){
        _functionCode = MODBUS_FC_WRITE_SINGLE_COIL;
        return;
    }
    if(arg1 == WRITE_REGISTER){
        _functionCode = MODBUS_FC_WRITE_SINGLE_REGISTER;
        return;
    }
    if(arg1 == WRITE_MULTIPLE_COIL){
        _functionCode = MODBUS_FC_WRITE_MULTIPLE_COILS;
        return;
    }
    if(arg1 == WRITE_MULTIPLE_REGISTER){
        _functionCode = MODBUS_FC_WRITE_MULTIPLE_REGISTERS;
        return;
    }
}

void modbusrequest::on_spnStartAddress_valueChanged(int arg1)
{
    _address = arg1;
}

void modbusrequest::on_spnCoil_valueChanged(int arg1)
{
    _coil = arg1;
}

void modbusrequest::on_pushButton_clicked()
{
    emit request_ready(_slave, _functionCode, _address, _coil);
}
