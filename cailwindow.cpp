#include "cailwindow.h"
#include "ui_cailwindow.h"
#include <QDebug>

CailWindow::CailWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CailWindow)
{
    ui->setupUi(this);
    ui->pushButton_back->setVisible(false);
    ui->pushButton_down->setVisible(false);
    ui->pushButton_left->setVisible(false);
    ui->pushButton_level->setVisible(false);
    ui->pushButton_right->setVisible(false);
    ui->pushButton_up->setVisible(false);
    ui->pushButton_Acc->setText("Acc Cail");
    this->adjustSize();
    this->setFixedSize(this->width(), this->height());
    this->setWindowFlags(this->windowFlags() &(~Qt::WindowMaximizeButtonHint));
    _motor_manage = new motor_manage(_motor1, _motor2);

    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(timer_update()));
    _timer->start(10);
}

CailWindow::~CailWindow()
{
    delete ui;
}

void CailWindow::on_pushButton_self_clicked()
{
//    _motor1.self_start();
//    _motor2.self_start();
    _motor_manage->self_start();
}

void CailWindow::on_pushButton_cail_clicked()
{
    _motor_manage->cail_start();
    qDebug("on_pushButton_cail_clicked");
}

void CailWindow::timer_update()
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
    _motor_manage->update();

}

void CailWindow::on_pushButton_Acc_clicked()
{
    _motor_manage->acc_cail_next();
//    enum pilot_cail _status;
//    //qDebug("on_pushButton_cail_clicked");
//    _status = _motor_manage->acc_cail_status();
//    if((PILOT_CAIL_START==_status) || (PILOT_CAIL_LEVEL==_status) || (PILOT_CAIL_UP==_status) ||\
//       (PILOT_CAIL_DOWN==_status) || (PILOT_CAIL_LEFT==_status) || (PILOT_CAIL_RIGHT==_status) || (PILOT_CAIL_BACK==_status))
//    {
//        qDebug("Next");
//        ui->pushButton_Acc->setText("Next");
//        _motor_manage->acc_cail_next();
//    }
//    else if(PILOT_CAIL_DONE==_status)
//    {
//        qDebug("Done");
//        ui->pushButton_Acc->setText("Done");
//    }
//    else
//    {
//        qDebug("Cail");
//        ui->pushButton_Acc->setText("Acc Cail");
//        _motor_manage->acc_cail_start();
//    }
}
