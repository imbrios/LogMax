#-------------------------------------------------
#
# Project created by QtCreator 2020-07-24T16:44:47
#
#-------------------------------------------------

QT       += core gui serialport printsupport serialbus network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = app
TEMPLATE = app
target.path = /home/root
INSTALLS += target
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        app.cpp \
        libmodbus/modbus.c \
        serialport.cpp \
        modbusrequest.cpp \
        modbus_form.cpp \
        libmodbus/modbus-data.c \
        libmodbus/modbus-tcp.c \
        libmodbus/modbus-rtu.c \
        digital.cpp \
        uart.cpp \
        qcustomplot/qcustomplot.cpp \
        analog.cpp \
        rtutempsensor.cpp \
        iot.cpp \
        settings.cpp

HEADERS += \
        app.h \
        libmodbus/modbus.h \
        serialport.h \
        modbusrequest.h \
        modbus_form.h \
        digital.h \
        uart.h \
        qcustomplot/qcustomplot.h \
        analog.h \
        rtutempsensor.h \
        libmodbus/modbus-version.h \
        libmodbus/modbus-tcp.h \
        libmodbus/modbus-rtu.h \
        libmodbus/modbus-rtu-private.h \
        libmodbus/modbus-private.h \
        libmodbus/config.h \
        config.h \
        iot.h \
        settings.h

INCLUDEPATH += libmodbus libpaho-mqtt3c
#libpaho-mqtt3a libpaho-mqtt3as libpaho-mqtt3cs

LIBS += -lpaho-mqtt3c -lsoc

FORMS += \
        app.ui \
        serialport.ui \
        modbusrequest.ui \
        modbus_form.ui \
        digital.ui \
        uart.ui \
        analog.ui \
        iot.ui \
        settings.ui

RESOURCES += \
    images.qrc
