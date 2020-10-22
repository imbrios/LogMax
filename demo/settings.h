#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QPainter>

namespace Ui {
class settings;
}

class settings : public QWidget
{
    Q_OBJECT

public:
    explicit settings(QWidget *parent = 0);
    ~settings();

private slots:
    void on_btnExit_clicked();

private:
    Ui::settings *ui;
    void paintEvent(QPaintEvent* event);
};

#endif // SETTINGS_H
