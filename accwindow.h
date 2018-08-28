#ifndef ACCWINDOW_H
#define ACCWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTimer>

#include "backthread.h"

namespace Ui {
class AccWindow;
}

class AccWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AccWindow(QWidget *parent = 0);
    ~AccWindow();

private slots:
    void on_pushButton_self_clicked();

    void on_pushButton_cail_clicked();

    void on_pushButton_Acc_clicked();

public slots:
    void timer_update();

private:
    Ui::AccWindow *ui;

    QTimer* _timer;
    backThread _backThread;
};

#endif // ACCWINDOW_H
