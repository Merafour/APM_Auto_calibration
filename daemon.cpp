#include "daemon.h"

//#include "motor_obj.h"

//motor_obj motor1;
//motor_obj motor2;
//Motor_obj *motor;

Daemon::Daemon(QObject *parent) : QObject(parent)
{

}

void Daemon::doWork(const int delay)
{
    int nValue=0;

    _motor1.init(MOTOR_NUM1);
    _motor2.init(MOTOR_NUM2);
    QThread::msleep(delay);
    while(1)
    {
        ++nValue;
        qDebug("doWork... %d\r\n", nValue);
        QThread::msleep(500);
    }
}

void Daemon::_clicked_slots(const int _num, const int _event)
{
    qDebug("motor%d ... %d\r\n", _num+1, _event);
    if(MOTOR_NUM1==_num)
    {
        motor = &_motor1;
    }
    else if(MOTOR_NUM2==_num)
    {
        motor = &_motor2;
    }
    else
    {
        return ;
    }
//    switch (_event) {
//    case DAEMON_EVENT_STOP:
//        motor->en(1);
//        motor->pwm(0);
//        break;
//    case DAEMON_EVENT_FORWARD:
//        motor->en(1);
//        motor->dir(1);
//        motor->pwm(100);
//        break;
//    case DAEMON_EVENT_REVERSE:
//        motor->en(1);
//        motor->dir(0);
//        motor->pwm(100);
//        break;
//    default:
//        break;
//    }
}

void Daemon::do_operations(const int delay)
{
    //qDebug("operations %d ... %d\r\n", _num+1, _event);
    qDebug("do_operations %d\r\n", delay);
}

void Daemon::on_clicked()
{
    qDebug("on_clicked \r\n");
}
