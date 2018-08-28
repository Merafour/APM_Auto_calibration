#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //_motor.init();
    _motor_manage = new motor_manage(_motor1, _motor2);
//    _motor1.init(MOTOR_NUM1);
//    _motor2.init(MOTOR_NUM2);
    //test = 0;
//    _controller = new Controller();
//    _controller->start("COM3");
    _daemon = new ControllerDaemon();
    //_daemon->start();

    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(timer_update()));
    _timer->start(10);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//#define   TEST_MOTER     0

void MainWindow::on_pushButton_T_clicked()
{
//#if 1==TEST_MOTER
//    _motor.motor1_en(1);
//    _motor.motor1_pwm(0);
//#else
//    _motor.motor2_en(1);
//    _motor.motor2_pwm(0);
//#endif
    motor = &_motor1;
    motor->en(1);
    motor->pwm(0);
    //_daemon->clicked(MOTOR_NUM1, DAEMON_EVENT_STOP);
}

void MainWindow::on_pushButton_B_clicked()
{
//#if 1==TEST_MOTER
//    _motor.motor1_en(1);
//    _motor.motor1_dir(0);
//    _motor.motor1_pwm(100);
//#else
//    _motor.motor2_en(1);
//    _motor.motor2_dir(0);
//    _motor.motor2_pwm(100);
//#endif
    motor = &_motor1;
    motor->en(1);
    motor->dir(MOTOR_DIR_REV);
    motor->pwm(100);
    //_daemon->clicked(MOTOR_NUM1, DAEMON_EVENT_REVERSE);
}

void MainWindow::on_pushButton_F_clicked()
{
//#if 1==TEST_MOTER
//    _motor.motor1_en(1);
//    _motor.motor1_dir(1);
//    _motor.motor1_pwm(100);
//#else
//    _motor.motor2_en(1);
//    _motor.motor2_dir(1);
//    _motor.motor2_pwm(100);
//#endif
    motor = &_motor1;
    motor->en(1);
    motor->dir(MOTOR_DIR_FOR);
    motor->pwm(300);
    //_daemon->clicked(MOTOR_NUM1, DAEMON_EVENT_FORWARD);
}

void MainWindow::on_pushButton_B_2_clicked()
{
    motor = &_motor2;
    motor->en(1);
    motor->dir(MOTOR_DIR_REV);
    motor->pwm(300, 3200);
    //_daemon->clicked(MOTOR_NUM2, DAEMON_EVENT_REVERSE);
}

void MainWindow::on_pushButton_F_2_clicked()
{
    motor = &_motor2;
    motor->en(1);
    motor->dir(MOTOR_DIR_FOR);
    motor->pwm(300, 3200);
    //_daemon->clicked(MOTOR_NUM2, DAEMON_EVENT_FORWARD);
}

void MainWindow::on_pushButton_T_2_clicked()
{
    motor = &_motor2;
    motor->en(1);
    motor->pwm(0, 0);
    //_daemon->clicked(MOTOR_NUM2, DAEMON_EVENT_STOP);
}

void MainWindow::on_pushButton_self_clicked()
{
    // zijian
    //_daemon->user();
//    _motor1.self_start();
//    _motor2.self_start();
    _motor_manage->self_start();
}

void MainWindow::on_pushButton_cail_clicked()
{
    // jiaozhun
    _motor_manage->cail_start();
    qDebug("on_pushButton_cail_clicked");
}


//void MainWindow::result_slots(const daemon_cmd cmd, const int value)
//{
//    switch (cmd) {
//    case DAEMON_CMD_IN_M1:
//        ui->label_in_m1->setText("motor1 input:"+QString::number(value));
//        break;
//    case DAEMON_CMD_IN_M2:
//        ui->label_in_m2->setText("motor2 input:"+QString::number(value));
//        break;
//    case DAEMON_CMD_UPDATE:
//        ;
//        break;
//    default:
//        break;
//    }
//}

// 10 ms
void MainWindow::timer_update()
{
    int check1=0;
    int check2=0;
    _motor_manage->self();
    check1 = _motor1.self();
    check2 = _motor2.self();
    //qDebug("timer_update \r\n");
    ui->label_in_m1->setText("motor1 input:"+QString::number(_motor1.signal_in()) + " check:"+QString::number(check1));
    ui->label_in_m2->setText("motor2 input:"+QString::number(_motor2.signal_in()) + " check:"+QString::number(check2));
    _motor_manage->cail();

}


void MainWindow::on_pushButton_M1_0deg_clicked()
{
    _motor1.deg0(300);
}

void MainWindow::on_pushButton_M1_90deg_clicked()
{
    _motor1.deg90(300);
}

void MainWindow::on_pushButton_M1_180deg_clicked()
{
    _motor1.deg180(300);
}

void MainWindow::on_pushButton_4_m1_270deg_clicked()
{
    _motor1.deg270(300);
}

void MainWindow::on_pushButton_M2_0deg_clicked()
{
    _motor2.deg0(300);
}

void MainWindow::on_pushButton_M2_90deg_clicked()
{
    _motor2.deg90(300);
}

void MainWindow::on_pushButton_M2_180deg_clicked()
{
    _motor2.deg180(300);
}

void MainWindow::on_pushButton_M2_270deg_clicked()
{
    _motor2.deg270(300);
}

void MainWindow::on_pushButton_M1_self_clicked()
{
    _motor1.self_start();
}

void MainWindow::on_pushButton_10_clicked()
{
    _motor2.self_start();
}

void MainWindow::on_pushButton_M2_90e_clicked()
{
    _motor2.deg90e(300);
}

void MainWindow::on_pushButton_M1_90e_clicked()
{
    _motor1.deg90e(300);
}

void MainWindow::on_pushButton_level_clicked()
{
    _motor_manage->pilot_level();
}

void MainWindow::on_pushButton_up_clicked()
{
    _motor_manage->pilot_up();
}

void MainWindow::on_pushButton_down_clicked()
{
    _motor_manage->pilot_down();
}

void MainWindow::on_pushButton_left_clicked()
{
    _motor_manage->pilot_left();
}

void MainWindow::on_pushButton_rught_clicked()
{
    _motor_manage->pilot_right();
}

void MainWindow::on_pushButton_back_clicked()
{
    _motor_manage->pilot_back();
}
