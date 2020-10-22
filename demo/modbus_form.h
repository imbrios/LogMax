#ifndef MODBUS_FORM_H
#define MODBUS_FORM_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPainter>
#include <QPlainTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include "serialport.h"
#include "modbusrequest.h"
#include "libmodbus/modbus.h"

namespace Ui {
class modbus_form;
}

class modbus_form : public QWidget
{
    Q_OBJECT

public:
    explicit modbus_form(QWidget *parent = 0);
    ~modbus_form();

public slots:
    void send_request(int slave, int functioncode, int address, int no_of_coils);
    void update_port();

private slots:

    void on_btnConnect_toggled(bool checked);

    void on_btnExec_clicked();

    void on_btnExit_clicked();

private:
    Ui::modbus_form *ui;
    QPushButton *exit();
    QPlainTextEdit *txtRead;
    QPlainTextEdit *txtWrite;
    QLabel *lblStatus;
    serialport *port;
    modbusrequest *request;
    modbus_t * _modbus;
    bool _connected;
    QTimer *_pollTimer;
    QList<int> writeData;
    uint8_t *tab_rq_bits;
    uint8_t *tab_rp_bits;
    uint16_t *tab_rq_registers;
    uint16_t *tab_rw_rq_registers;
    uint16_t *tab_rp_registers;
    QString _serialport;
    QChar _parity;
    int _baud;
    int _dataBits;
    int _stopBits;
    int _timeOut;

    void begin(QString port, int baud, char parity, int databit, int stopbit);
    void end();
    void read(int slave, int functionCode, int startAddress, int noOfItems);
    void write(int slave, int startAddress, int noOfItems, uint8_t *data);
    void write(int slave, int startAddress, int noOfItems, uint16_t *data);
    void write(int slave, int startAddress, uint8_t coilStatus);
    void write(int slave, int startAddress, uint16_t registerValue);
    void paintEvent(QPaintEvent* event);

signals:
    void quit();
    void m_error(QString error);
};

#endif // MODBUS_FORM_H
