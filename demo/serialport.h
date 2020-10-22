#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QWidget>

namespace Ui {
class serialport;
}

class serialport : public QWidget
{
    Q_OBJECT

public:
    explicit serialport(QWidget *parent = 0);
    ~serialport();
    QString get_port();
    int get_data();
    int get_baud();
    int get_stopbit();
    QChar get_parity();
    void setSerialPortItem(QStringList listItems);

private slots:
    void on_btnSet_clicked();

    void on_cmbPort_currentIndexChanged(const QString &arg1);

    void on_cmbbaud_currentIndexChanged(int index);

    void on_cmbbaud_currentIndexChanged(const QString &arg1);

    void on_cmbdata_currentIndexChanged(const QString &arg1);

    void on_cmbparity_currentIndexChanged(const QString &arg1);

    void on_cmbstop_currentIndexChanged(const QString &arg1);

private:
    Ui::serialport *ui;
    QString _port;
    int _baud;
    int _databit;
    QChar _parity;
    int _stopbit;

signals:
    void port_update();
};

#endif // SERIALPORT_H
