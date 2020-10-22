#include "rtutempsensor.h"

rtutempsensor::rtutempsensor()
{
    _run = false;
}

rtutempsensor::~rtutempsensor()
{
    _run = false;
    free(query);
    modbus_mapping_free(mb_mapping);
    modbus_free(ctx);

}
void rtutempsensor::begin(QString port, int baud)
{
    ctx = modbus_new_rtu(port.toLatin1().constData(), baud, 'N', 8, 1);// 0);
    if(ctx == NULL)
    {
        qDebug() << "Error server modbus on port" << port << "with slave id " << id;
        return;
    }
    modbus_set_slave(ctx, id);
    query = (uint8_t *) malloc(MODBUS_RTU_MAX_ADU_LENGTH * sizeof(uint8_t));
    memset(query, 0, MODBUS_RTU_MAX_ADU_LENGTH * sizeof(uint8_t));
    header_length = modbus_get_header_length(ctx);
    modbus_set_debug(ctx, TRUE);
    mb_mapping = modbus_mapping_new(0, 0, 2, 0);
    if (mb_mapping == NULL) {
        qDebug() <<  "Failed to allocate the mapping: %s\n" << modbus_strerror(errno);
        modbus_free(ctx);
        return;
    }
    if (modbus_connect(ctx) == -1) {
        qDebug() << "Unable to connect %s\n" << modbus_strerror(errno);
        modbus_free(ctx);
        return;
    }
    mb_mapping->tab_registers[0] = qrand() % 100;
    _run = true;
    qDebug() << "RTU temp modbus server ready";
    //modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
}


void rtutempsensor::listen()
{
    int ret = 0;
    while(_run){
        do {
            ret = modbus_receive(ctx, query);
            /* Filtered queries return 0 */
        } while (ret == 0);
        if (ret == -1 && errno != EMBBADCRC) {
            /* Quit */
            qDebug() << "Quit %s\n";
            break;
        }
        if(query[header_length] == 0x03){

        }
        qDebug() << "Replying with query";
        ret = modbus_reply(ctx, query, ret, mb_mapping);
        if (ret == -1) {
            qDebug() << "Error in reply %s\n";
            break;
        }
    }
    this->quit();
    this->wait();
}

void rtutempsensor::run()
{
    listen();
}
