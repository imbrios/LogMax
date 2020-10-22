#ifndef ANALOG_H
#define ANALOG_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QVector>
#include "qcustomplot/qcustomplot.h"
#include "config.h"

namespace Ui {
class analog;
}

class analog : public QWidget
{
    Q_OBJECT

public:
    explicit analog(QWidget *parent = 0);
    ~analog();

private:
    Ui::analog *ui;
    QTimer *updatePlot;
    // Add data:
    QVector<double> ticks;
    QVector<double> adc;
    QCPBars *_adcBar;
    double _adc[4];
    void clearData();
    void plotdata();
    void readInternalADC();
    void drawBarGraph();
    void paintEvent(QPaintEvent* event);

private slots:
    void updatePlotSlot();
    void on_btnExit_clicked();
};

#endif // ANALOG_H
