#ifndef IOT_H
#define IOT_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <MQTTClient.h>
#include <QTimer>
namespace Ui {
class iot;
}

class iot : public QWidget
{
    Q_OBJECT

public:
    explicit iot(QWidget *parent = 0);
    ~iot();

private slots:
    void on_btnExit_clicked();

    void on_btnConnect_toggled(bool checked);

    void on_btnSub_clicked();

    void on_btnSend_clicked();

    void on_payload_slot();

private:
    Ui::iot *ui;

    MQTTClient client;
    QString *server;
    int port;
    int mode;
    QString *topic;
    bool connected;
    QTimer *updateMessage;

    void begin(QString server, int port);
    void stop();
    void send(QString topic, QString message);
    void subcribe(QString topic);
    static int on_message_recv(void *context, char *topicName, int topicLen, MQTTClient_message *message);
    //static int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message);
    void paintEvent(QPaintEvent* event);

signals:
    void onRecv(QString payload);
};

#endif // IOT_H
