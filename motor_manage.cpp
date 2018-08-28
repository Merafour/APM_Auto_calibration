#include "motor_manage.h"
#include <QThread>
#include <QDebug>

#include <string.h>

#include "gcs_mavlink.h"

static GCS_MAVLINK gcs;
static pthread_t myThread;
static void *serialThread (void *arg)
{
  (void)arg;
  for (;;)
  {
    //qDebug("serialThread");
    gcs.update();
      //gcs.test();
    QThread::msleep(50);
  }


  return NULL ;
}




//motor_manage::motor_manage(QObject *parent) : QObject(parent)
//{
//}

motor_manage::motor_manage(Motor_obj &_level, Motor_obj &_vertical, QObject *parent) : QObject(parent), motor_level(_level), motor_vertical(_vertical)
{
    int res ;
    motor_level.init(MOTOR_NUM1);
    motor_vertical.init(MOTOR_NUM2);
    _cail = PILOT_CAIL_NULL;
    _self = 0;
    _acc_cail = PILOT_CAIL_NULL;

    res = 0;//pthread_create (&myThread, NULL, serialThread, NULL) ;
    qDebug("motor_manage res:%d", res);
}



void motor_manage::cail()
{
    int check1=0;
    int check2=0;

    check1 = motor_level.self();
    check2 = motor_vertical.self();
    if((0!=check1) || (0!=check2))
    {
        _cail = PILOT_CAIL_NULL;
        return;
    }
    //qDebug("motor_manage::cail()");
    switch (_cail) {
    case PILOT_CAIL_START:
        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()))
        {
            //qDebug("PILOT_CAIL_START");
            //QThread::msleep(1000);
            pilot_level();
            //_cail = PILOT_CAIL_UP;
            _cail = PILOT_CAIL_STEP1;
            gcs.calibration_acc_start();
        }
        break;
    case PILOT_CAIL_UP:
        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()))
        {
            qDebug("PILOT_CAIL_UP");
            QThread::msleep(1000);
            pilot_up();
            _cail = PILOT_CAIL_LEFT;
        }
        break;
    case PILOT_CAIL_LEFT:
        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()))
        {
            qDebug("PILOT_CAIL_LEFT");
            QThread::msleep(1000);
            pilot_left();
            _cail = PILOT_CAIL_RIGHT;
        }
        break;
    case PILOT_CAIL_RIGHT:
        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()))
        {
            qDebug("PILOT_CAIL_RIGHT");
            QThread::msleep(1000);
            pilot_right();
            _cail = PILOT_CAIL_DOWN;
        }
        break;
    case PILOT_CAIL_DOWN:
        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()))
        {
            qDebug("PILOT_CAIL_DOWN");
            QThread::msleep(1000);
            pilot_down();
            _cail = PILOT_CAIL_BACK;
        }
        break;
    case PILOT_CAIL_BACK:
        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()))
        {
            qDebug("PILOT_CAIL_BACK");
            QThread::msleep(1000);
            pilot_back();
            _cail = PILOT_CAIL_LEVEL;
        }
        break;
    case PILOT_CAIL_LEVEL:
        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()))
        {
            qDebug("PILOT_CAIL_LEVEL");
            QThread::msleep(1000);
            pilot_level();
            _cail = PILOT_CAIL_END;
        }
        break;
    case PILOT_CAIL_END:
        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()))
        {
            qDebug("PILOT_CAIL_END");
            _cail = PILOT_CAIL_DONE;
        }
        break;
    case PILOT_CAIL_STEP1:
        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()))
        {
            //qDebug("PILOT_CAIL_DOWN");
            QThread::msleep(1000);
            pilot_left();//pilot_up();
            _cail = PILOT_CAIL_STEP2;
        }
        break;
    case PILOT_CAIL_STEP2:
        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()))
        {
            //qDebug("PILOT_CAIL_LEFT");
            QThread::msleep(1000);
            pilot_right();//pilot_left();
            _cail = PILOT_CAIL_STEP3;
        }
        break;
    case PILOT_CAIL_STEP3:
        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()))
        {
            //qDebug("PILOT_CAIL_RIGHT");
            QThread::msleep(1000);
            pilot_up();//pilot_right();
            _cail = PILOT_CAIL_STEP4;
        }
        break;
    case PILOT_CAIL_STEP4:
        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()))
        {
            //qDebug("PILOT_CAIL_DOWN");
            QThread::msleep(1000);
            pilot_down();
            _cail = PILOT_CAIL_STEP5;
        }
        break;
    case PILOT_CAIL_STEP5:
        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()))
        {
            //qDebug("PILOT_CAIL_BACK");
            QThread::msleep(1000);
            pilot_back();
            _cail = PILOT_CAIL_STEP6;
        }
        break;
    case PILOT_CAIL_STEP6:
        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()))
        {
            //qDebug("PILOT_CAIL_LEVEL");
            QThread::msleep(1000);
            pilot_level();
            _cail = PILOT_CAIL_END;
        }
        break;
//    case PILOT_CAIL_END:
//        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()))
//        {
//            qDebug("PILOT_CAIL_END");
//            _cail = PILOT_CAIL_DONE;
//        }
//        break;
    default:
        break;
    }
}

void motor_manage::update()
{
    char text[150];
    memset(text, 0, sizeof(text));
    gcs.get_text(text);

}

void motor_manage::acc_cail_start()
{
    qDebug("acc_cail_start");
    gcs.clear_text();
    pilot_level();
//    gcs.calibration_acc_start();
    _acc_cail = PILOT_CAIL_START;
}

void motor_manage::acc_cail_next()
{
    gcs.calibration_acc_next();
//    int check1=0;
//    int check2=0;
//    char text[150];

//    qDebug("acc_cail_next");
//    check1 = motor_level.self();
//    check2 = motor_vertical.self();
//    if((0!=check1) || (0!=check2))
//    {
//        _acc_cail = PILOT_CAIL_NULL;
//        return;
//    }
//    qDebug("acc_cail_next...");
//    memset(text, 0, sizeof(text));
//    gcs.get_text(text);
//    if (NULL!=strstr(text, "Calibration FAILED"))
//    {
//        _acc_cail = PILOT_CAIL_FAIL;
//        qDebug("PILOT_CAIL_FAIL");
//        return;
//    }
//    qDebug("acc_cail_next....");
//    //qDebug("motor_manage::cail()");
//    switch (_acc_cail) {
//    case PILOT_CAIL_START:
//        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()))
//        {
//            //pilot_level();
//            gcs.calibration_acc_start();
//            _acc_cail = PILOT_CAIL_LEVEL;
//        }
//        break;
//    case PILOT_CAIL_LEVEL:
//        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()) && (NULL!=strstr(text, "Place vehicle level")) )
//        {
//            gcs.clear_text();
//            gcs.calibration_acc_next();
//        }
//        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()) && (NULL!=strstr(text, "Place vehicle on its LEFT side")) )
//        {
//            pilot_left();
//            _acc_cail = PILOT_CAIL_LEFT;
//        }
//        break;
//    case PILOT_CAIL_LEFT:
//        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()) && (NULL!=strstr(text, "Place vehicle on its LEFT side")) )
//        {
//            gcs.clear_text();
//            gcs.calibration_acc_next();
//        }
//        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()) && (NULL!=strstr(text, "Place vehicle on its RIGHT side")) )
//        {
//            pilot_right();
//            _acc_cail = PILOT_CAIL_RIGHT;
//        }
//        break;
//    case PILOT_CAIL_RIGHT:
//        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()) && (NULL!=strstr(text, "Place vehicle on its RIGHT side")) )
//        {
//            gcs.clear_text();
//            gcs.calibration_acc_next();
//        }
//        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()) && (NULL!=strstr(text, "Place vehicle nose DOWN")) )
//        {
//            pilot_down();
//            _acc_cail = PILOT_CAIL_DOWN;
//        }
//        break;
//    case PILOT_CAIL_DOWN:
//        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()) && (NULL!=strstr(text, "Place vehicle nose DOWN")) )
//        {
//            gcs.clear_text();
//            gcs.calibration_acc_next();
//        }
//        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()) && (NULL!=strstr(text, "Place vehicle nose UP")) )
//        {
//            pilot_down();
//            _acc_cail = PILOT_CAIL_UP;
//        }
//        break;
//    case PILOT_CAIL_UP:
//        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()) && (NULL!=strstr(text, "Place vehicle nose UP")) )
//        {
//            gcs.clear_text();
//            gcs.calibration_acc_next();
//        }
//        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()) && (NULL!=strstr(text, "Place vehicle on its BACK")) )
//        {
//            pilot_back();
//            _acc_cail = PILOT_CAIL_BACK;
//        }
//        break;
//    case PILOT_CAIL_BACK:
//        if((0==motor_level.get_mark()) && (0==motor_vertical.get_mark()) && (NULL!=strstr(text, "Place vehicle on its BACK")) )
//        {
//            //qDebug("PILOT_CAIL_BACK");
//            gcs.clear_text();
//            gcs.calibration_acc_next();
//        }
//        if(NULL!=strstr(text, "Calibration successful"))
//        {
//            _acc_cail = PILOT_CAIL_DONE;
//            pilot_level();
//        }
//        break;
//    default:
//        break;
//    }
}

void motor_manage::self()
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
            _self=0;
        }
        break;
    default:
        break;
    }
}
