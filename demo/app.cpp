#include "app.h"
#include "ui_app.h"
#include "QBoxLayout"
#include <QThread>

#include <stdio.h>
#include <stdlib.h>
#include <ftdi.h>

app::app(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::app)
{
    ui->setupUi(this);
    timer_1s = new QTimer(this);
    QObject::connect(timer_1s, SIGNAL(timeout()), this, SLOT(on_time_update()));
    timer_1s->start(1000);
}

app::~app()
{
    delete ui;
}

void app::on_time_update()
{
    ui->lblTime->setText(QTime::currentTime().toString("hh:mm:ss"));
    ui->lblDate->setText(QDate::currentDate().toString("dd-MMMM-yyyy"));
}

void app::on_BTN_MOD_clicked()
{
    modbus_tab = new modbus_form(this);
    modbus_tab->setAttribute(Qt::WA_DeleteOnClose);
    modbus_tab->showFullScreen();
}

void app::on_BTN_DIGITAL_clicked()
{
    io_tab = new digital(this);
    io_tab->setAttribute(Qt::WA_DeleteOnClose);
    io_tab->showFullScreen();
}

void app::on_BTN_SERIAL_clicked()
{
    uart_tab = new uart(this);
    uart_tab->setAttribute(Qt::WA_DeleteOnClose);
    uart_tab->showFullScreen();
}

void app::on_BTN_ANALOG_clicked()
{
    adc_tab = new analog(this);
    adc_tab->setAttribute(Qt::WA_DeleteOnClose);
    adc_tab->showFullScreen();
}

void app::on_BTN_MQTT_clicked()
{
    mqtt_tab = new iot(this);
    mqtt_tab->setAttribute(Qt::WA_DeleteOnClose);
    mqtt_tab->showFullScreen();
}

void app::on_BTN_SETTINGS_clicked()
{
    setting_tab = new settings(this);
    setting_tab->setAttribute(Qt::WA_DeleteOnClose);
    setting_tab->showFullScreen();
}
