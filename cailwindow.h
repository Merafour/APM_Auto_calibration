#ifndef CAILWINDOW_H
#define CAILWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTimer>

#include "motor_obj.h"
#include "motor_manage.h"

namespace Ui {
class CailWindow;
}

class CailWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CailWindow(QWidget *parent = 0);
    ~CailWindow();

private slots:
    void on_pushButton_self_clicked();

    void on_pushButton_cail_clicked();

    void on_pushButton_Acc_clicked();

public slots:
    void timer_update();
private:
    Ui::CailWindow *ui;

    QTimer* _timer;
    Motor_obj_level _motor1;
    Motor_obj _motor2;
    Motor_obj *motor;
    motor_manage *_motor_manage;
};

#endif // CAILWINDOW_H
