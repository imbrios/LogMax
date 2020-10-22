#ifndef DIGITAL_H
#define DIGITAL_H

#include <QWidget>
#include <QPainter>
#include <libsoc_pwm.h>
#include "libsoc_debug.h"
#include "config.h"
#include <libsoc_gpio.h>
namespace Ui {
class digital;
}

class digital : public QWidget
{
    Q_OBJECT

public:
    explicit digital(QWidget *parent = 0);
    ~digital();

private slots:
    void on_btnExit_clicked();

    void on_btn102_toggled(bool checked);

    void on_btn45_toggled(bool checked);

    void on_btn99_toggled(bool checked);

    void on_btn104_toggled(bool checked);

    void scanInput();

    void on_slpwm0_valueChanged(int value);

    void on_slpwm1_valueChanged(int value);

    void on_slpwm2_valueChanged(int value);

    void on_slpwm3_valueChanged(int value);

private:
    Ui::digital *ui;
    gpio *output_1;
    gpio *output_2;
    gpio *output_3;
    gpio *output_4;
    gpio *input_1;
    gpio *input_2;
    gpio *input_3;
    gpio *input_4;
    pwm *pwm_ch0;
    pwm *pwm_ch1;
    pwm *pwm_ch2;
    pwm *pwm_ch3;
    int pwm_period;

    QTimer *scanInputTimer;
    void init_pwm();
    void paintEvent(QPaintEvent* event);
};

#endif // DIGITAL_H
