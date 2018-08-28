#include <string.h>

#include "backthread.h"
#include "motor_obj.h"
#include "gcs_mavlink.h"
#include "quadrotor.h"
#include <softPwm.h>
#include <softTone.h>
#include <wiringPi.h>
#include <QProcess>

Quadrotor quad;
static GCS_MAVLINK gcs;

static void *backgroundThread (void *arg);
static QString _status="";
//static enum ERR _enumber=ERR_NULL;
static backThread::ERR _enumber=backThread::ERR::ERR_NULL;
#define BEEP         1
#define LED_RUN      4
#define LED_FMU      2
#define LED_TICK     5000
#define KEY          3
#define KEY_POWER    0

backThread::backThread(QObject *parent) : QObject(parent)
{
    quad._self = 0;
    quad._axle = 0;
    _enumber = ERR_NULL;
}

backThread::~backThread()
{
    pthread_cancel (myThread) ;
    pthread_join   (myThread, NULL) ;
    softToneStop (BEEP);
    softToneStop (LED_RUN);
//    softToneStop (LED_FMU);
}

void backThread::start()
{
    quad.init();
    pthread_create (&myThread, NULL, backgroundThread, NULL) ;
//    softPwmCreate (1, 0, 50) ;
//    softPwmWrite  (1, 0) ;
    softToneCreate (BEEP);
//    softToneWrite (1, 5000);
    softToneWrite (BEEP, 0);
    softPwmCreate (LED_RUN, 0, LED_TICK) ;
    softPwmWrite  (LED_RUN, LED_TICK) ;
//    softPwmCreate (LED_FMU, 0, LED_TICK) ;
//    softPwmWrite  (LED_FMU, LED_TICK) ;
    //digitalWrite (LED_RUN, HIGH) ;
//    digitalWrite (LED_RUN, LOW) ;
//    pinMode      (LED_RUN, OUTPUT) ;
//    digitalWrite (LED_RUN, LOW) ;
    digitalWrite (LED_FMU, HIGH) ;
    pinMode      (LED_FMU, OUTPUT) ;
    digitalWrite (LED_FMU, HIGH) ;
    digitalWrite (KEY, HIGH) ;
    pinMode      (KEY, INPUT) ;
    digitalWrite (KEY_POWER, HIGH) ;
    pinMode      (KEY_POWER, INPUT) ;
}

void backThread::update()
{
    ;
}

void backThread::beep_on()
{
    softToneWrite (BEEP, 5000);
    //softPwmWrite  (1, 10) ;
}

void backThread::beep_off()
{
    softToneWrite (BEEP, 0);
    //softPwmWrite  (1, 0) ;
    //softToneStop (1);
    _enumber=ERR_NULL;
}

int backThread::key()
{
    return digitalRead(KEY);
}

int backThread::led(int flag)
{
    if(1==flag)
    {
        softPwmWrite  (LED_RUN, LED_TICK/2) ;
    }
    else
    {
        softPwmWrite  (LED_RUN, 1) ;
    }
    return 0;
}

int backThread::led_fmu(int flag)
{
    if(1==flag)
    {
        digitalWrite (LED_FMU, HIGH) ;
    }
    else
    {
        digitalWrite (LED_FMU, LOW) ;
    }
    return 0;
}

int backThread::heart()
{
    return gcs.recv_heart;
}

uint32_t backThread::boot_ms()
{
    return gcs.time_boot_ms;
}

int backThread::fmu_exist()
{
    int fmu = gcs.fmu_exist();
    if(fmu>=0) return 1;
    return 0;
}

void backThread::self_start()
{
    quad._self=1;
}

void backThread::axle_test()
{
    quad._axle = 1;
}

void backThread::acc_cail_start()
{
    quad.acc_cail_step = Quadrotor::Acc_Cail::STEP_START;
    _enumber=ERR_CAIL;
}

//int backThread::self()
//{
//    return quad._self;
//}

//int backThread::cail()
//{
//    return quad.acc_cail_step;
//}

void backThread::read(int &check_l, int &check_v, int &signal_l, int &signal_v)
{
    check_l = quad.check_l;
    check_v = quad.check_v;
    signal_l = quad.signal_l;
    signal_v = quad.signal_v;
}

void backThread::read_tune(int &vtn, int &vtn_, int &_tv, int &ltn, int &ltn_, int &_tl)
{
    quad.vertical()->tune(vtn, vtn_, _tv);
    quad.level()->tune(ltn, ltn_, _tl);
}

void backThread::set_tune(const int _tv, const int _tl)
{
    quad.vertical()->set_tune(_tv);
    quad.level()->set_tune(_tl);
}

QString backThread::status()
{
    return _status;
}

backThread::ERR backThread::err_number()
{
    return _enumber;
}

void backThread::err_number_clear()
{
    _enumber = ERR_NULL;
}

static void *backgroundThread (void *arg)
{
  (void)arg;
    int count=0;
    int key_power=0;
    //gcs.open_fmu();
  for (;;)
  {
    //qDebug("backgroundThread");
    //gcs.update();
      //gcs.test();
      _status = "Standby";
    quad.update();
    if(0==digitalRead(KEY_POWER))
    {
        key_power++;
    }
    else
    {
        key_power=0;
    }
    if(key_power>20)  // 20*100ms = 2s
    {
        //qDebug("sudo shutdown now");
        //QProcess process;
        //process.start("sudo shutdown now");
        //process.start("/home/pi/power_off.sh &");
        //system("/home/pi/power_off.sh &");
        QProcess::startDetached("sudo shutdown now");
        //exit(0);
        key_power=0;
    }
    if(Quadrotor::Acc_Cail::STEP_START == quad.acc_cail_step)
    {
#if 0
        _status = "to zero";
        quad.acc_cail_step = Quadrotor::Acc_Cail::STEP_ZERO;
        quad._self=1;
        while(1)
        {
            quad.update();
            if(5==quad._self) // done
            {
                break;
            }
        }
        _status = "connect to FMU";
        quad.acc_cail_step = Quadrotor::Acc_Cail::STEP_FMU;
        if(0!=gcs.open_fmu())
        {
            gcs.close_fmu();
            _status = "connect to FMU Fail";
            _enumber = backThread::ERR::ERR_FMU;
            backThread::beep_on();
            continue;
        }
        _status = "watting heartbeat";
        for(count=0; count<1000; count++) // 10s
        {
            QThread::msleep(10);
            gcs.update();
            if(gcs.recv_heart)
            {
                gcs.data_stream();
                break;
            }
        }
        if(count>=1000)
        {
            gcs.close_fmu();
            _status = "no recv heartbeat";
            _enumber = backThread::ERR::ERR_HEART;
            backThread::beep_on();
            continue;
        }
#else
        _status = "connect to FMU";
        quad.acc_cail_step = Quadrotor::Acc_Cail::STEP_FMU;
        for(count=0; count<30; count++) // 3s
        {
            if(0==gcs.open_fmu())
            {
                break;
            }
        }
        if(count>=30)
        {
            gcs.close_fmu();
            _status = "connect to FMU Fail";
            _enumber = backThread::ERR::ERR_FMU;
            backThread::beep_on();
            continue;
        }
//        if(0!=gcs.open_fmu())
//        {
//            gcs.close_fmu();
//            _status = "connect to FMU Fail";
//            _enumber = backThread::ERR::ERR_FMU;
//            backThread::beep_on();
//            continue;
//        }
#if 0
        for(count=0; count<1000; count++) // 10s
        {
            QThread::msleep(10);
            gcs.update();
        }
        gcs.close_fmu();
        _enumber=backThread::ERR::ERR_NULL;
        quad.acc_cail_step = Quadrotor::Acc_Cail::STEP_NULL;
        continue;
#endif
        //_status = "watting heartbeat";
        _status = "watting system start";
        for(count=0; count<5000; count++) // 50s
        {
            QThread::msleep(10);
            gcs.update();
            //if(gcs.recv_heart>2)
            if(gcs.time_boot_ms>10000) // 10s
            {
                gcs.data_stream();
                break;
            }
        }
        if(count>=5000)
        {
            gcs.close_fmu();
            _status = "no recv heartbeat";
            _enumber = backThread::ERR::ERR_HEART;
            backThread::beep_on();
            continue;
        }
        _status = "to zero";
        quad.acc_cail_step = Quadrotor::Acc_Cail::STEP_ZERO;
        quad._self=1;
        while(1)
        {
            QThread::msleep(50);
            quad.update();
            if(5==quad._self) // done
            {
                break;
            }
        }
#endif
         _status = "Acc_Cail";
        qDebug("Quadrotor::Acc_Cail::STEP_LEVEL");
        //quad.acc_cail_step = Quadrotor::Acc_Cail::STEP_LEVEL;
        quad.pilot_level();
        gcs.update();
        gcs.clear_text();
        gcs.calibration_acc_start();
        //while(1)
        {
            int timeout=0;
            int timeout_count=0;
            timeout=0;
            gcs.update();
            for(count=0; count<6000; count++) // 60s
            {
                QThread::msleep(10);
                if(0==timeout) timeout_count=0;
                timeout++;
                gcs.update();
                if(timeout>=101) // 1s
                {
                    timeout=1;
                    timeout_count++;
                    if(timeout_count<8) // <8s
                    {
                        gcs.clear_text();
                        if(Quadrotor::Acc_Cail::STEP_START == quad.acc_cail_step)
                        {
                            gcs.calibration_acc_start();
                        }
                        else
                        {
                            gcs.calibration_acc_next();
                        }
                        continue;
                    }
                    //break; // timeout
                    _status = "Calibration TimeOut";
                    _enumber = backThread::ERR::ERR_TIMEOUT;
                    backThread::beep_on();
                    gcs.clear_text();
                    QThread::msleep(500);
                    break;
                }
                if(backThread::ERR::ERR_NULL==_enumber)
                {
                    _status = "Calibration FAILED";
                    _enumber = backThread::ERR::ERR_STOP;
                    quad.level()->stop();
                    quad.vertical()->stop();
                    gcs.clear_text();
                    QThread::msleep(500);
                    break;
                }
//                gcs.update();
                if(NULL!=strstr(gcs._text(), "Place vehicle level"))
                {
                     _status = "Place vehicle level";
                     quad.acc_cail_step = Quadrotor::Acc_Cail::STEP_LEVEL;
                    qDebug("Quadrotor::Acc_Cail::STEP_LEVEL");
                    gcs.clear_text();
                    gcs.calibration_acc_next();
                    timeout=0;
                    continue;//break;
                }
                if(NULL!=strstr(gcs._text(), "Place vehicle on its LEFT side"))
                {
                     _status = "Place vehicle on its LEFT side";
                     quad.acc_cail_step = Quadrotor::Acc_Cail::STEP_LEFT;
                    qDebug("Quadrotor::Acc_Cail::STEP_LEFT");
                    gcs.clear_text();
                    quad.pilot_left();
                    gcs.calibration_acc_next();
                    timeout=0;
                    continue;//break;
                }
                if(NULL!=strstr(gcs._text(), "Place vehicle on its RIGHT side"))
                {
                     _status = "Place vehicle on its RIGHT side";
                     quad.acc_cail_step = Quadrotor::Acc_Cail::STEP_RIGHT;
                    qDebug("Quadrotor::Acc_Cail::STEP_RIGHT");
                    gcs.clear_text();
                    quad.pilot_right();
                    gcs.calibration_acc_next();
                    timeout=0;
                    continue;//break;
                }
                if(NULL!=strstr(gcs._text(), "Place vehicle nose DOWN"))
                {
                     _status = "Place vehicle nose DOWN";
                     quad.acc_cail_step = Quadrotor::Acc_Cail::STEP_DOWN;
                    qDebug("Quadrotor::Acc_Cail::STEP_DOWN");
                    gcs.clear_text();
                    quad.pilot_down();
                    gcs.calibration_acc_next();
                    timeout=0;
                    continue;//break;
                }
                if(NULL!=strstr(gcs._text(), "Place vehicle nose UP"))
                {
                     _status = "Place vehicle nose UP";
                     quad.acc_cail_step = Quadrotor::Acc_Cail::STEP_UP;
                    qDebug("Quadrotor::Acc_Cail::STEP_UP");
                    gcs.clear_text();
                    quad.pilot_up();
                    gcs.calibration_acc_next();
                    timeout=0;
                    continue;//break;
                }
                if(NULL!=strstr(gcs._text(), "Place vehicle on its BACK"))
                {
                     _status = "Place vehicle on its BACK";
                     quad.acc_cail_step = Quadrotor::Acc_Cail::STEP_BACK;
                    qDebug("Quadrotor::Acc_Cail::STEP_BACK");
                    gcs.clear_text();
                    quad.pilot_back();
                    gcs.calibration_acc_next();
                    timeout=0;
                    continue;//break;
                }
                if(NULL!=strstr(gcs._text(), "Calibration successful"))
                {
                    _status = "Calibration successful";
                    _enumber = backThread::ERR::ERR_SUCC;
                    gcs.clear_text();
                    QThread::msleep(500);
                    break;
                }
                if(NULL!=strstr(gcs._text(), "Calibration FAILED"))
                {
                    _status = "Calibration FAILED";
                    _enumber = backThread::ERR::ERR_FAIL;
                    backThread::beep_on();
                    gcs.clear_text();
                    QThread::msleep(500);
                    break;
                }
            }
//            if(count>=1000)
//            {
//                gcs.close_fmu();
//                _status = "calibration fail";
//                continue;
//            }
            //break;
        }
        quad.acc_cail_step = Quadrotor::Acc_Cail::STEP_DONE;
        gcs.close_fmu();
        //qDebug("gcs.close_fmu ... %d, %d", count, timeout);
        qDebug("gcs.close_fmu ... %d", count);
        if(backThread::ERR::ERR_TIMEOUT == _enumber)
        {
            continue;
        }
        if(backThread::ERR::ERR_STOP != _enumber)
        {
            quad.pilot_level();
        }
        //qDebug("gcs.close_fmu ... %d, %d", count, timeout);
//        if((count>=2000) || (timeout>=500)) // 20s, 5s
//        {
//            _status = "Calibration TimeOut";
//            _enumber = backThread::ERR::ERR_TIMEOUT;
//            backThread::beep_on();
//            gcs.clear_text();
//            QThread::msleep(500);
//            break;
//        }
#if 0
        for(count=0; count<1000; count++) // 10s
        {
            QThread::msleep(10);
            gcs.update();
            if(NULL!=strstr(gcs._text(), "Place vehicle level"))
            {
                gcs.calibration_acc_next();
                break;
            }
        }
        if(count>=1000)
        {
            gcs.close_fmu();
            _status = "calibration fail";
            continue;
        }
#endif


        //gcs.close_fmu();
//        QThread::msleep(1000);
//        quad._axle = 1;
        if( (1!=quad.level()->signal_in()) || (1!=quad.vertical()->signal_in()))
        {
            _enumber = backThread::ERR::ERR_ZERO;
            backThread::beep_on();
        }
        if(backThread::ERR::ERR_SUCC == _enumber) _enumber = backThread::ERR::ERR_DONE;
    }
    QThread::msleep(100);
  }

  return NULL ;
}





