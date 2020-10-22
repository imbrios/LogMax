#include "serialport.h"
#include "ui_serialport.h"

serialport::serialport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::serialport)
{
    ui->setupUi(this);
    ui->cmbPort->setCurrentIndex(1);
    ui->cmbbaud->setCurrentIndex(ui->cmbbaud->findText("9600"));
    ui->cmbdata->setCurrentIndex(1);
    ui->cmbparity->setCurrentIndex(0);
    ui->cmbstop->setCurrentIndex(0);

    QPixmap close(":/icons/images/icons-save-close-40.png");
    QIcon closeIcon(close);
    ui->btnSet->setIcon(closeIcon);
    ui->btnSet->setIconSize(close.rect().size());
}

serialport::~serialport()
{
    delete ui;
}

void serialport::on_btnSet_clicked()
{
    QString parity = ui->cmbparity->currentText();
    _port = ui->cmbPort->currentText();
    _baud = ui->cmbbaud->currentText().toInt();
    _databit = ui->cmbdata->currentText().toInt();
    _parity = parity[0];
    _stopbit = ui->cmbstop->currentText().toInt();
    emit port_update();
}

void serialport::on_cmbPort_currentIndexChanged(const QString &arg1)
{
    _port=arg1;
}

void serialport::on_cmbbaud_currentIndexChanged(const QString &arg1)
{
    _baud = arg1.toInt();
}

void serialport::on_cmbdata_currentIndexChanged(const QString &arg1)
{
    _databit = arg1.toInt();
}

void serialport::on_cmbparity_currentIndexChanged(const QString &arg1)
{
    _parity = arg1[0];
}

void serialport::on_cmbstop_currentIndexChanged(const QString &arg1)
{
    _stopbit = arg1.toInt();
}

void serialport::on_cmbbaud_currentIndexChanged(int index)
{

}
QString serialport::get_port()
{
    return _port;
}

int serialport::get_baud()
{
    return _baud;
}

int serialport::get_data()
{
    return _databit;
}

QChar serialport::get_parity()
{
    return _parity;
}

int serialport::get_stopbit()
{
    return _stopbit;
}

void serialport::setSerialPortItem(QStringList listItems)
{
    ui->cmbPort->clear();
    ui->cmbPort->addItems(listItems);
}
