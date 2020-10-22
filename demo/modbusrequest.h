#ifndef MODBUSREQUEST_H
#define MODBUSREQUEST_H

#include <QWidget>

namespace Ui {
class modbusrequest;
}

class modbusrequest : public QWidget
{
    Q_OBJECT

public:
    explicit modbusrequest(QWidget *parent = 0);
    ~modbusrequest();

private slots:
    void on_spnSlave_valueChanged(int arg1);

    void on_cmbFunctionCode_currentIndexChanged(const QString &arg1);

    void on_spnStartAddress_valueChanged(int arg1);

    void on_spnCoil_valueChanged(int arg1);

    void on_pushButton_clicked();

private:
    Ui::modbusrequest *ui;
    int _slave;
    int _functionCode;
    int _address;
    int _coil;

signals:
    void request_ready(int slave, int functioncode, int address, int _noOfCoil);
};

#endif // MODBUSREQUEST_H
