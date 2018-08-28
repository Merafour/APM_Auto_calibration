#include "quadrotor.h"
#include <QThread>

Quadrotor::Quadrotor(QObject *parent) : QObject(parent)
{
    acc_cail_step = STEP_NULL;
}

void Quadrotor::init()
{
    motor_level.init(MOTOR_NUM1);
    motor_vertical.init(MOTOR_NUM2);
}

void Quadrotor::update()
{
    self();
    axle();
    signal_l = motor_level.signal_in();
    signal_v = motor_vertical.signal_in();
    check_l = motor_level.self();
    check_v = motor_vertical.self();
}

void Quadrotor::self()
{
    int check1=0;
    int check2=0;
    check1 = motor_level.self();
    check2 = motor_vertical.self();
    switch (_self) {
    case 1:  // start
        motor_vertical.self_start();
        _self=2;
        break;
    case 2:  // start
        check2 = motor_vertical.self();
        if((0==motor_vertical.get_mark()) && (1!=check2))
        {
            _self=3;
        }
        break;
    case 3:  // start
        motor_level.self_start();
        _self=4;
        break;
    case 4:  // start
        check1 = motor_level.self();
        if((0==motor_level.get_mark()) && (1!=check1))
        {
            _self=5; // OK
            break;
        }
        break;
    default:
        break;
    }
}

void Quadrotor::axle()
{
    int check1=0;
    int check2=0;

    check1 = motor_level.self();
    check2 = motor_vertical.self();
    if((0!=check1) || (0!=check2))
    {
        _axle = 0;
        return;
    }
    //qDebug("motor_manage::cail()");
    switch (_axle) {
    case 1: // start
        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()))
        {
            qDebug("PILOT_CAIL_START");
            pilot_level();
            _axle = 2;
        }
        break;
//    case PILOT_CAIL_END:
//        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()))
//        {
//            qDebug("PILOT_CAIL_END");
//            _cail = PILOT_CAIL_DONE;
//        }
//        break;
    case 2:
        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()))
        {
            //qDebug("PILOT_CAIL_DOWN");
            QThread::msleep(1000);
            pilot_left();//pilot_up();
            _axle = 3;
        }
        break;
    case 3:
        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()))
        {
            //qDebug("PILOT_CAIL_LEFT");
            QThread::msleep(1000);
            pilot_right();//pilot_left();
            _axle = 4;
        }
        break;
    case 4:
        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()))
        {
            //qDebug("PILOT_CAIL_RIGHT");
            QThread::msleep(1000);
            pilot_up();//pilot_right();
            _axle = 5;
        }
        break;
    case 5:
        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()))
        {
            //qDebug("PILOT_CAIL_DOWN");
            QThread::msleep(1000);
            pilot_down();
            _axle = 6;
        }
        break;
    case 6:
        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()))
        {
            //qDebug("PILOT_CAIL_BACK");
            QThread::msleep(1000);
            pilot_back();
            _axle = 7;
        }
        break;
    case 7:
        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()))
        {
            //qDebug("PILOT_CAIL_LEVEL");
            QThread::msleep(1000);
            pilot_level();
            _axle = 8;
        }
        break;
    case 8:
        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()))
        {
            qDebug("PILOT_CAIL_END");
            _axle = 9;
        }
        break;
    default:
        break;
    }
}
