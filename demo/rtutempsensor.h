#ifndef RTUTEMPSENSOR_H
#define RTUTEMPSENSOR_H

#include <QThread>
#include <QObject>
#include <QDebug>
#include "libmodbus/modbus.h"

class rtutempsensor : public QThread
{
    Q_OBJECT
public:
    explicit rtutempsensor();
    ~rtutempsensor();
    // overriding the QThread's run() method
    void run();

private:

    modbus_t *ctx;
    modbus_mapping_t *mb_mapping;
    int id;
    uint8_t *query;
    int header_length;
    bool _run;

    void begin(QString port, int baud);
signals:

public slots:
    void listen();
};

#endif // RTUTEMPSENSOR_H
