#include "digital.h"
#include "ui_digital.h"
#include <QTimer>
#include <QDebug>

digital::digital(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::digital)
{
    ui->setupUi(this);
    this->setAutoFillBackground(true);

    // exit button icon
    QPixmap close(":/icons/images/icons-close-48.png");
    QIcon closeIcon(close);
    ui->btnExit->setIcon(closeIcon);
    ui->btnExit->setIconSize(close.rect().size());
    //    // can be improved using Vector for simplicity init every output and input
    output_1 = libsoc_gpio_request(SODIMM_98, LS_GPIO_SHARED);
    if (output_1 != NULL)
    {
        // Set direction to OUTPUT
        libsoc_gpio_set_direction(output_1, OUTPUT);
        if (libsoc_gpio_get_direction(output_1) != OUTPUT)
        {
            qDebug() << "Failed to set direction to OUTPUT";
        }
    }
    output_2 = libsoc_gpio_request(SODIMM_133, LS_GPIO_SHARED);
    if (output_2 != NULL)
    {
        // Set direction to OUTPUT
        libsoc_gpio_set_direction(output_2, OUTPUT);
        if (libsoc_gpio_get_direction(output_2) != OUTPUT)
        {
            qDebug() << "Failed to set direction to OUTPUT";
        }
    }
    output_3 = libsoc_gpio_request(SODIMM_104, LS_GPIO_SHARED);
    if (output_3 != NULL)
    {
        // Set direction to OUTPUT
        libsoc_gpio_set_direction(output_3, OUTPUT);
        if (libsoc_gpio_get_direction(output_3) != OUTPUT)
        {
            qDebug() << "Failed to set direction to OUTPUT";
        }
    }
    output_4 = libsoc_gpio_request(SODIMM_99, LS_GPIO_SHARED);
    if (output_4 != NULL)
    {
        // Set direction to OUTPUT
        libsoc_gpio_set_direction(output_4, OUTPUT);
        if (libsoc_gpio_get_direction(output_4) != OUTPUT)
        {
            qDebug() << "Failed to set direction to OUTPUT";
        }
    }

    input_1 = libsoc_gpio_request(SODIMM_135, LS_GPIO_SHARED);
    if (input_1 != NULL)
    {
        // Set direction to OUTPUT
        libsoc_gpio_set_direction(input_1, INPUT);
        if (libsoc_gpio_get_direction(input_1) != INPUT)
        {
            qDebug() << "Failed to set direction to INPUT";
        }
    }
    input_2 = libsoc_gpio_request(SODIMM_105, LS_GPIO_SHARED);
    if (input_2 != NULL)
    {
        // Set direction to OUTPUT
        libsoc_gpio_set_direction(input_2, INPUT);
        if (libsoc_gpio_get_direction(input_2) != INPUT)
        {
            qDebug() << "Failed to set direction to INPUT";
        }
    }

    input_3 = libsoc_gpio_request(SODIMM_45, LS_GPIO_SHARED);
    if (input_3 != NULL)
    {
        // Set direction to OUTPUT
        libsoc_gpio_set_direction(input_3, INPUT);
        if (libsoc_gpio_get_direction(input_3) != INPUT)
        {
            qDebug() << "Failed to set direction to INPUT";
        }
    }
    input_4 = libsoc_gpio_request(SODIMM_102, LS_GPIO_SHARED);
    if (input_4 != NULL)
    {
        // Set direction to OUTPUT
        libsoc_gpio_set_direction(input_4, INPUT);
        if (libsoc_gpio_get_direction(input_4) != INPUT)
        {
            qDebug() << "Failed to set direction to INPUT";
        }
    }

    // timer to read input pin status
    scanInputTimer = new QTimer(this);
    connect(scanInputTimer, SIGNAL(timeout()), this, SLOT(scanInput()));
    scanInputTimer->start(100);

    // init four pwm channel one(backlight will not work)
    init_pwm();
}

digital::~digital()
{
    delete ui;
}

void digital::init_pwm()
{
    // using libsoc as an example on how to use libsoc API.
    pwm_ch0 = libsoc_pwm_request(0, 0, LS_PWM_SHARED);
    if (pwm_ch0)
    {
        libsoc_pwm_set_enabled(pwm_ch0, ENABLED);
        libsoc_pwm_set_period(pwm_ch0, pwm_period);
        ui->slpwm0->setValue(libsoc_pwm_get_duty_cycle(pwm_ch0));
        qDebug() << "Init pwm_ch0";

    }
    pwm_ch1 = libsoc_pwm_request(1, 0, LS_PWM_SHARED);
    if (pwm_ch1)
    {
        libsoc_pwm_set_enabled(pwm_ch1, ENABLED);
        libsoc_pwm_set_period(pwm_ch1, pwm_period);
        ui->slpwm1->setValue(libsoc_pwm_get_duty_cycle(pwm_ch1));
        qDebug() << "Init pwm_ch1";

    }
    pwm_ch2 = libsoc_pwm_request(2, 0, LS_PWM_SHARED);
    if (pwm_ch2)
    {
        libsoc_pwm_set_enabled(pwm_ch2, ENABLED);
        libsoc_pwm_set_period(pwm_ch2, pwm_period);
        ui->slpwm2->setValue(libsoc_pwm_get_duty_cycle(pwm_ch2));
        qDebug() << "Init pwm_ch2";

    }
    pwm_ch3 = libsoc_pwm_request(3, 0, LS_PWM_SHARED);
    if (pwm_ch3)
    {
        libsoc_pwm_set_enabled(pwm_ch3, ENABLED);
        libsoc_pwm_set_period(pwm_ch3, pwm_period);
        ui->slpwm3->setValue(libsoc_pwm_get_duty_cycle(pwm_ch3));
        qDebug() << "Init pwm_ch3";

    }
}
void digital::scanInput()
{
    QPixmap ledPix(":/icons/images/icons-led-36.png");
    QPixmap ledPixOff(":/icons/images/icons-led-off-36.png");
    if(libsoc_gpio_get_level(input_1) == HIGH){
        ui->ind98->setPixmap(ledPix);
        qDebug() << "High input 1";
    }
    else{
        ui->ind98->setPixmap(ledPixOff);
    }

    if(libsoc_gpio_get_level(input_2) == HIGH){
        ui->ind105->setPixmap(ledPix);
    }
    else{
        ui->ind105->setPixmap(ledPixOff);
    }

    if(libsoc_gpio_get_level(input_3) == HIGH){
        ui->ind133->setPixmap(ledPix);
    }
    else{
        ui->ind133->setPixmap(ledPixOff);
    }

    if(libsoc_gpio_get_level(input_4) == HIGH){
        ui->ind135->setPixmap(ledPix);
    }
    else{
        ui->ind135->setPixmap(ledPixOff);
    }
}
void digital::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}

void digital::on_btnExit_clicked()
{

    scanInputTimer->stop();
    libsoc_gpio_free(input_1);
    libsoc_gpio_free(input_2);
    libsoc_gpio_free(input_3);
    libsoc_gpio_free(input_4);
    libsoc_gpio_free(output_1);
    libsoc_gpio_free(output_2);
    libsoc_gpio_free(output_3);
    libsoc_gpio_free(output_4);
    // clean PWM
    libsoc_pwm_free(pwm_ch0);
    libsoc_pwm_free(pwm_ch1);
    libsoc_pwm_free(pwm_ch2);
    libsoc_pwm_free(pwm_ch3);
    this->close();
}


void digital::on_btn102_toggled(bool checked)
{
    if(checked){
        //output_1->digitalWrite(gpio::HIGH);
        libsoc_gpio_set_level(output_1, HIGH);
        qDebug() << "output_1 value HIGH";
    }
    else{
        //output_1->digitalWrite(gpio::LOW);
        libsoc_gpio_set_level(output_1, LOW);
        qDebug() << "output_1 value LOW";
    }
}

void digital::on_btn45_toggled(bool checked)
{
    if(checked){
        libsoc_gpio_set_level(output_2, HIGH);
        qDebug() << "output_2 value HIGH";
    }
    else{
        libsoc_gpio_set_level(output_2, LOW);
        qDebug() << "output_2 value LOW";
    }
}

void digital::on_btn99_toggled(bool checked)
{
    if(checked){
        libsoc_gpio_set_level(output_3, HIGH);
        qDebug() << "output_3 value HIGH";
    }
    else{
        libsoc_gpio_set_level(output_3, LOW);
        qDebug() << "output_3 value LOW";
    }
}

void digital::on_btn104_toggled(bool checked)
{
    if(checked){
        libsoc_gpio_set_level(output_4, HIGH);
        qDebug() << "output_4 value HIGH";
    }
    else{
        libsoc_gpio_set_level(output_4, LOW);
        qDebug() << "output_4 value LOW";
    }
}

void digital::on_slpwm0_valueChanged(int value)
{
    qDebug() << "PWM 0 value " << value;
    libsoc_pwm_set_duty_cycle(pwm_ch0, value);
}

void digital::on_slpwm1_valueChanged(int value)
{
    qDebug() << "PWM 1 value " << value;
    libsoc_pwm_set_duty_cycle(pwm_ch1, value);
}

void digital::on_slpwm2_valueChanged(int value)
{
    qDebug() << "PWM 2 value " << value;
    libsoc_pwm_set_duty_cycle(pwm_ch2, value);
}

void digital::on_slpwm3_valueChanged(int value)
{
    qDebug() << "PWM 3 value " << value;
    libsoc_pwm_set_duty_cycle(pwm_ch3, value);
}
