#ifndef UART_H
#define UART_H

#include <QWidget>
#include <QPainter>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QPlainTextEdit>
#include <QFrame>
#include <QLabel>
#include "serialport.h"

namespace Ui {
class uart;
}

class uart : public QWidget
{
    Q_OBJECT

public:
    explicit uart(QWidget *parent = 0);
    ~uart();
public slots:
    void update_port();
private:
    Ui::uart *ui;
    serialport *portsettings;
    QString _serialport;
    int _baud;
    QChar _parity;
    int _dataBits;
    int _stopBits;
    int _rts;
    int _timeOut;

    QSerialPort *serial;
    QByteArray serialData;
    QString serialBuffer;
    QStringList portList;
    QFrame *txtFrame;
    QPlainTextEdit *txtBoxRead;
    QPlainTextEdit *txtBoxWrite;
    QLabel *lblstatus;
    bool is_available;
    void paintEvent(QPaintEvent* event);

signals:
    void quit();
    void dataRead(QString);
private slots:
    void on_btnExit_clicked();
    void readSerial();
    void on_btnOpen_toggled(bool checked);
    void on_btnClear_clicked();
    void on_btnSend_clicked();
    void update_read_ui(QString);
};

#endif // UART_H
