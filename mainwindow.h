#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTimer>

//#include "motor.h"
#include "motor_obj.h"
#include "worker.h"
#include "daemon.h"
#include "motor_manage.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_pushButton_T_clicked();

    void on_pushButton_B_clicked();

    void on_pushButton_F_clicked();

    void on_pushButton_B_2_clicked();

    void on_pushButton_F_2_clicked();

    void on_pushButton_T_2_clicked();

    void on_pushButton_self_clicked();

    void on_pushButton_cail_clicked();

    void on_pushButton_M1_0deg_clicked();

    void on_pushButton_M1_90deg_clicked();

    void on_pushButton_M1_180deg_clicked();

    void on_pushButton_4_m1_270deg_clicked();

    void on_pushButton_M2_0deg_clicked();

    void on_pushButton_M2_90deg_clicked();

    void on_pushButton_M2_180deg_clicked();

    void on_pushButton_M2_270deg_clicked();

    void on_pushButton_M1_self_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_M2_90e_clicked();

    void on_pushButton_M1_90e_clicked();

    void on_pushButton_level_clicked();

    void on_pushButton_up_clicked();

    void on_pushButton_down_clicked();

    void on_pushButton_left_clicked();

    void on_pushButton_rught_clicked();

    void on_pushButton_back_clicked();

public slots:
//        void result_slots(const enum daemon_cmd cmd, const int value);
    void timer_update();

private:
    Ui::MainWindow *ui;
    QTimer* _timer;
    //motor _motor;
    //Motor_obj _motor1;
    Motor_obj_level _motor1;
    Motor_obj _motor2;
    Motor_obj *motor;
//    int test;

//    Controller *_controller;
    ControllerDaemon *_daemon;
    motor_manage *_motor_manage;
};

#endif // MAINWINDOW_H
