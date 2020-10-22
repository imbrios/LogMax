#ifndef APP_H
#define APP_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include "modbus_form.h"
#include "digital.h"
#include "uart.h"
#include "analog.h"
#include "iot.h"
#include "settings.h"
namespace Ui {
class app;
}

class app : public QMainWindow
{
    Q_OBJECT

public:
    explicit app(QWidget *parent = 0);
    ~app();

private slots:
    //void on_pushButton_4_clicked();

    void on_BTN_MOD_clicked();

    void on_BTN_DIGITAL_clicked();

    void on_time_update();

    void on_BTN_SERIAL_clicked();

    //void on_BTN_CAMERA_clicked();

    void on_BTN_ANALOG_clicked();

    void on_BTN_MQTT_clicked();

    void on_BTN_SETTINGS_clicked();

private:
    Ui::app *ui;
    modbus_form *modbus_tab;
    settings *setting_tab;
    analog *adc_tab;
    digital *io_tab;
    uart *uart_tab;
    iot *mqtt_tab;
    QTimer *timer_1s;
};

#endif // APP_H
