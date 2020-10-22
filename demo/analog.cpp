#include "analog.h"
#include "ui_analog.h"

analog::analog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::analog)
{
    ui->setupUi(this);

    // default bar graph init
    drawBarGraph();

    // timer to read four channel adc and update graph
    updatePlot = new QTimer(this);
    connect(updatePlot, SIGNAL(timeout()), this, SLOT(updatePlotSlot()));
    updatePlot->start(500);

    // setting exit button icon
    QPixmap close(":/icons/images/icons-close-48.png");
    QIcon closeIcon(close);
    ui->btnExit->setIcon(closeIcon);
    ui->btnExit->setIconSize(close.rect().size());
}

analog::~analog()
{
    delete ui;
}

void analog::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}

void analog::drawBarGraph()
{
    // create empty bar chart objects:
    _adcBar = new QCPBars(ui->plot->xAxis, ui->plot->yAxis);
    _adcBar->setAntialiased(false);
    _adcBar->setStackingGap(1);
    _adcBar->setPen(QPen(QColor(0, 168, 140).lighter(130)));
    _adcBar->setBrush(QColor(0, 168, 140));
    _adcBar->setName("Analog Raw");

    // prepare y axis:
    ui->plot->yAxis->setRange(0, 4095);

    // Prepare x axis:
    QVector<QString> labels;
    ticks << 1 << 2 << 3 << 4;
    labels << "CH 1" << "CH 2" << "CH 3" << "CH 4";
}

void analog::readInternalADC()
{
    char *rawAdc[]={IMX7_ADC0,
                    IMX7_ADC1,
                    IMX7_ADC2,
                    IMX7_ADC3};

    for(int ch=0; ch<4; ch++)
    {
        QFile file(rawAdc[ch]);
        QString string;
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            string = in.readLine();
            _adc[ch]=string.toDouble();
            qDebug() << "read adc: " << _adc[ch] ;
        }
        file.close();
    }
}

void analog::updatePlotSlot()
{
    QVector<double> ladc;
    readInternalADC();
    ladc   << _adc[0] << _adc[1] << _adc[2] << _adc[3] ;
    _adcBar->setData(ticks, ladc);
    ui->plot->replot();
}

void analog::on_btnExit_clicked()
{
    updatePlot->stop();
    this->close();
}

