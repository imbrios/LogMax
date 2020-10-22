#include "iot.h"
#include "MQTTClient.h"
#include "ui_iot.h"


// static function, not clean way to do it need to fix same to include in class
static int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    char* payload = (char *)message->payload;
    qDebug() << "Received operation" << payload;
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

iot::iot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::iot)
{
    ui->setupUi(this);
    // exit button icon
    QPixmap close(":/icons/images/icons-close-48.png");
    QIcon closeIcon(close);
    ui->btnExit->setIcon(closeIcon);
    ui->btnExit->setIconSize(close.rect().size());
    // mqtt client
    client = nullptr;

    updateMessage = new QTimer(this);
    connect(updateMessage, SIGNAL(timeout()), this, SLOT(on_payload_slot()));
    updateMessage->start(1);
}

iot::~iot()
{
    delete ui;
}

void iot::begin(QString server, int port)
{
    int rc;
    if(!client){
        qDebug() << "connecting server" <<  server;
        rc = MQTTClient_create(&client, "tcp://mqtt.eclipse.org:1883", "logmax", MQTTCLIENT_PERSISTENCE_NONE, NULL);
        qDebug() << "connecting error tag" <<  server;
        if(rc != MQTTCLIENT_SUCCESS){
            qDebug() << "Error in creating mqtt client";
            return;
        }
        MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
        //conn_opts.username = "<<tenant_ID>>/<<username>>";
        //conn_opts.password = "<<password>>";
        conn_opts.keepAliveInterval = 10;
        conn_opts.cleansession = 1;
        MQTTClient_setCallbacks(client, NULL, NULL, on_message_recv, NULL);
        if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
            qDebug() << "Failed to connect, return code "<< rc;
            return;
        }
        connected = true;
    }
}

void iot::stop()
{
    MQTTClient_disconnect(client, 1000);
    MQTTClient_destroy(&client);
}

void iot::send(QString topic, QString message)
{
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = (char *)message.toLatin1().constData();
    pubmsg.payloadlen = strlen((char*)pubmsg.payload);
    pubmsg.qos = 2;
    pubmsg.retained = 0;
    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(client, topic.toLatin1().constData(), &pubmsg, &token);
    MQTTClient_waitForCompletion(client, token, 1000L);
    qDebug() << "Message " <<  message << token << " delivered ";
}

void iot::subcribe(QString topic)
{
    MQTTClient_subscribe(client, topic.toLatin1().constData(), 0);
}
void iot::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}

void iot::on_btnExit_clicked()
{
    this->close();
}

void iot::on_btnConnect_toggled(bool checked)
{
    if(checked){
        begin(ui->txtServer->text(), ui->txtPort->text().toInt());
        if(connected){
            ui->btnConnect->setText("Disconnect");
        }
    }
    else{
        if(client){
            stop();
            if(connected == false){
                ui->btnConnect->setText("Connect");
            }
        }
    }
}

void iot::on_btnSub_clicked()
{
    subcribe(ui->txtTopix->text());
}

void iot::on_btnSend_clicked()
{
    if(ui->txtPayload){
        send(ui->txtTopix->text(), ui->txtPayload->text());
    }
}

int iot:: on_message_recv(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    char* payload = (char *)message->payload;
    qDebug() << "Received operation" << payload;
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void iot::on_payload_slot()
{
    ui->lnDisplay->setText("Hello");
}
