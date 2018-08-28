#include "motor_obj.h"
#include <wiringPi.h>
//#include <softPwm.h>
#include <QDebug>
//#include <QThread>

#include "motor_pwm.h"



int Motor_obj::pwmRange = 500;
int Motor_obj::setup=0;
Motor_obj* Motor_obj::_obj[MOTOR_NULL];

Motor_obj::Motor_obj(QObject *parent) : QObject(parent)
{
    degree = 0;
}


/**
 +-----+-----+---------+------+---+---Pi 3+--+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 |     |     |    3.3v |      |   |  1 || 2  |   |      | 5v      |     |     | 1
 |   2 |   8 |   SDA.1 |   IN | 1 |  3 || 4  |   |      | 5v      |     |     | 2
 |   3 |   9 |   SCL.1 |   IN | 1 |  5 || 6  |   |      | 0v      |     |     | 3
 |   4 |   7 | GPIO. 7 |   IN | 1 |  7 || 8  | 1 | ALT0 | TxD     | 15  | 14  | 4
 |     |     |      0v |      |   |  9 || 10 | 1 | ALT0 | RxD     | 16  | 15  | 5
 |  17 |   0 | GPIO. 0 |   IN | 0 | 11 || 12 | 0 | OUT  | GPIO. 1 | 1   | 18  | 6
 |  27 |   2 | GPIO. 2 |   IN | 0 | 13 || 14 |   |      | 0v      |     |     | 7
 |  22 |   3 | GPIO. 3 |   IN | 0 | 15 || 16 | 0 | IN   | GPIO. 4 | 4   | 23  | 8
 |     |     |    3.3v |      |   | 17 || 18 | 0 | IN   | GPIO. 5 | 5   | 24  | 9
 |  10 |  12 |    MOSI |   IN | 0 | 19 || 20 |   |      | 0v      |     |     | 10
 |   9 |  13 |    MISO |   IN | 0 | 21 || 22 | 0 | IN   | GPIO. 6 | 6   | 25  | 11
 |  11 |  14 |    SCLK |   IN | 0 | 23 || 24 | 1 | IN   | CE0     | 10  | 8   | 12
 |     |     |      0v |      |   | 25 || 26 | 1 | IN   | CE1     | 11  | 7   | 13
 |   0 |  30 |   SDA.0 |   IN | 1 | 27 || 28 | 1 | IN   | SCL.0   | 31  | 1   | 14
 |   5 |  21 | GPIO.21 |   IN | 1 | 29 || 30 |   |      | 0v      |     |     | 15
 |   6 |  22 | GPIO.22 |   IN | 1 | 31 || 32 | 0 | IN   | GPIO.26 | 26  | 12  | 16
 |  13 |  23 | GPIO.23 |   IN | 0 | 33 || 34 |   |      | 0v      |     |     | 17
 |  19 |  24 | GPIO.24 |   IN | 0 | 35 || 36 | 0 | IN   | GPIO.27 | 27  | 16  | 18
 |  26 |  25 | GPIO.25 |   IN | 0 | 37 || 38 | 0 | IN   | GPIO.28 | 28  | 20  | 19
 |     |     |      0v |      |   | 39 || 40 | 0 | IN   | GPIO.29 | 29  | 21  | 20
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+---Pi 3+--+---+------+---------+-----+-----+

*/
#if 0
// wPi
#define MOTER_VERTICAL     31   // moter 1
#define MOTER_VERTICAL_EN  10   // moter 1 enable
#define MOTER_VERTICAL_IN  4   // moter 1 input
#define MOTER_VERTICAL_DIR 11   // moter 1 dir

#define MOTER_LEVEL        27  // moter 2
#define MOTER_LEVEL_EN     29  // moter 2 enable
//#define MOTER_LEVEL_IN     26  // moter 2 input
#define MOTER_LEVEL_IN     5  // moter 2 input
#define MOTER_LEVEL_DIR    28  // moter 2 dir
#else
// wPi
#define MOTER_VERTICAL     25   // moter 1
#define MOTER_VERTICAL_EN  23   // moter 1 enable
#define MOTER_VERTICAL_IN  22   // moter 1 input
#define MOTER_VERTICAL_DIR 24   // moter 1 dir
#define MOTER_VERTICAL_TUNE  9   // tune +
#define MOTER_VERTICAL_TUNE_ 7   // tune -

#define MOTER_LEVEL        27  // moter 2
#define MOTER_LEVEL_EN     29  // moter 2 enable
#define MOTER_LEVEL_IN     26  // moter 2 input
#define MOTER_LEVEL_DIR    28  // moter 2 dir
#define MOTER_LEVEL_TUNE   21   // tune +
#define MOTER_LEVEL_TUNE_  30   // tune -
#endif

static motor_pin_t motor_res[2] =
{
    {
        .pin = MOTER_VERTICAL,
        .pin_en = MOTER_VERTICAL_EN,
        .pin_in = MOTER_VERTICAL_IN,
        .pin_dir = MOTER_VERTICAL_DIR,
        .tune = MOTER_VERTICAL_TUNE,
        .tune_ = MOTER_VERTICAL_TUNE_,
        .pwmRange = 500,
    },
    {
        .pin = MOTER_LEVEL,
        .pin_en = MOTER_LEVEL_EN,
        .pin_in = MOTER_LEVEL_IN,
        .pin_dir = MOTER_LEVEL_DIR,
        .tune = MOTER_LEVEL_TUNE,
        .tune_ = MOTER_LEVEL_TUNE_,
        .pwmRange = 500,
    },
};
int Motor_obj::init(enum motor_num _num)
{
//    if(MOTOR_NUM1==_num)
//    {
//        pin = MOTER_VERTICAL;
//        pin_en = MOTER_VERTICAL_EN;
//        pin_in = MOTER_VERTICAL_IN;
//        pin_dir = MOTER_VERTICAL_DIR;
//    }
//    else
//    {
//        pin = MOTER_LEVEL;
//        pin_en = MOTER_LEVEL_EN;
//        pin_in = MOTER_LEVEL_IN;
//        pin_dir = MOTER_LEVEL_DIR;
//    }
    degree = 0;
    _self = SELE_NULL;
    _motor_num = _num;
    _obj[_motor_num]=this;
    motor_res[_motor_num].pwmRange = pwmRange;
    set_mode(MOTOR_MODE_RESET);

    qDebug ("motor::init wiringPiSetup\n") ;

    if(0==setup)
    {
        if (wiringPiSetup () == -1)
          exit (1) ;
        setup=1;
    }
    qDebug ("motor::init()\n") ;
    motorPwmCreate(&motor_res[_motor_num], 0);  //创建软PWM
    pinMode (motor_res[_motor_num].pin_en, OUTPUT) ;
    digitalWrite (motor_res[_motor_num].pin_en, LOW) ;	// Off
    //digitalWrite (pin_en, HIGH) ;	// On
    pinMode (motor_res[_motor_num].pin_dir, OUTPUT) ;
    //digitalWrite (pin_dir, LOW) ;	// Off
    digitalWrite (motor_res[_motor_num].pin_dir, HIGH) ;	// On
    pinMode (motor_res[_motor_num].pin_in, INPUT) ;	//delayMicroseconds (1) ;
    pinMode (motor_res[_motor_num].tune, INPUT) ;
    pinMode (motor_res[_motor_num].tune_, INPUT) ;
    delayMicroseconds (1) ;
    btn = 1;
    btn_ = 1;
    return 0;
}

void Motor_obj::en(int _en)
{
    _en = ~_en;
    _en = _en&0x1;
    digitalWrite (motor_res[_motor_num].pin_en, _en) ;	// On
}

void Motor_obj::dir(enum motor_dir _dir)
{
    digitalWrite (motor_res[_motor_num].pin_dir, _dir) ;	// On
}

void Motor_obj::pwm(int _pwm)
{
    motorPwmWrite(motor_res[_motor_num].pin, _pwm, 1600);

}

void Motor_obj::pwm(int _pwm, int pulse)
{
    motorPwmWrite(motor_res[_motor_num].pin, _pwm, pulse);
}

void Motor_obj::stop()
{
    motorPwmWrite(motor_res[_motor_num].pin, 0, 0);
}

void Motor_obj::deg_rotate(const int _pwm,const int _degree)
{
    int pulse=0;
    int _deg=0;
    //qDebug ("\n----------------------------------") ;
    //qDebug ("deg_rotate _pwm:%d _deg:%d", _pwm, _degree) ;
    _deg = _degree-degree;
    //qDebug ("deg_rotate degree:%d _deg:%d", degree, _deg) ;
    if(_deg>=0)
    {
        //dir(MOTOR_DIR_FOR);
        dir(MOTOR_DIR_REV);
        //qDebug ("MOTOR_DIR_REV") ;
    }
    else
    {
        //dir(MOTOR_DIR_REV);
        dir(MOTOR_DIR_FOR);
        _deg = -_deg;
        //qDebug ("MOTOR_DIR_FOR") ;
    }
    degree = _degree;
    pulse = deg2pulse(_deg);
    //qDebug ("deg_rotate degree:%d _deg:%d pulse:%d", degree, _deg, pulse) ;
    set_mode(MOTOR_MODE_RUN);
    motorPwmWrite(motor_res[_motor_num].pin, _pwm, pulse);
}

void Motor_obj::set_mode(enum motor_mode _mode)
{
    motor_set_mode(motor_res[_motor_num].pin, _mode);
}

enum motor_mode Motor_obj::get_mode()
{
    return motor_get_mode(motor_res[_motor_num].pin);
}

int Motor_obj::get_mark()
{
    return get_marks(motor_res[_motor_num].pin);
}

int Motor_obj::signal_in()
{
    return motor_signal_in(motor_res[_motor_num].pin);
}

void Motor_obj::tune(int &tn, int &tn_, int &_t)
{
    tn  = digitalRead(motor_res[_motor_num].tune);
    tn_ = digitalRead(motor_res[_motor_num].tune_);

    if((0==btn) && (1==tn)) // UP
    {
        _tune++;
    }
    if((0==btn_) && (1==tn_)) // UP
    {
        _tune--;
    }
    btn = tn;
    btn_ = tn_;
    _t = _tune;
}

int Motor_obj::self()
{
    switch (_self) {
    case SELE_START:
        if(1==signal_in())
        {
            _self = SELE_STEP6;
            break;
        }
#if 0
        dir(MOTOR_DIR_FOR);
        set_mode(MOTOR_MODE_RUN);
        motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(5)); // 5deg
#else
        dir(MOTOR_DIR_REV);
        set_mode(MOTOR_MODE_RUN);
        motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(10)); // 5deg
#endif
        _self = SELE_STEP1;
        break;
    case SELE_STEP1:
#if 0
        if(0==get_marks(motor_res[_motor_num].pin))
        {
            dir(MOTOR_DIR_FOR);
            set_mode(MOTOR_MODE_RESET);
            motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(180)); // 180deg
            _self = SELE_STEP2;
        }
#else
        if(0==get_marks(motor_res[_motor_num].pin))
        {
            dir(MOTOR_DIR_FOR);
            set_mode(MOTOR_MODE_RESET);
            motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(380)); // 360deg
            _self = SELE_STEP6;
        }
#endif
        break;
    case SELE_STEP2:
        if(0==get_marks(motor_res[_motor_num].pin))
        {
            int pulse = get_pulse(motor_res[_motor_num].pin);
            int _deg = pulse2deg(pulse);
            if(_deg>(5+5))
            {
                _self = SELE_DONE;
                return 0;
            }
            else
            {
                dir(MOTOR_DIR_REV);
                set_mode(MOTOR_MODE_RUN);
                motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(5)); // 5deg
                _self = SELE_STEP2_AUX;
            }
        }
        break;
    case SELE_STEP2_AUX:
        if(0==get_marks(motor_res[_motor_num].pin))
        {
            dir(MOTOR_DIR_REV);
            set_mode(MOTOR_MODE_RESET);
            motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(360+5)); // 180deg
            _self = SELE_STEP3;
        }
        break;

    case SELE_STEP3:
        if(0==get_marks(motor_res[_motor_num].pin))
        {
            dir(MOTOR_DIR_FOR);
            set_mode(MOTOR_MODE_RUN);
            motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(5)); // 5deg
            _self = SELE_STEP3_AUX;
        }
        break;
    case SELE_STEP3_AUX:
        if(0==get_marks(motor_res[_motor_num].pin))
        {
            dir(MOTOR_DIR_FOR);
            set_mode(MOTOR_MODE_RESET);
            motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(360+5)); // 3200 ==> 180deg
            _self = SELE_STEP4;
        }
        break;
    case SELE_STEP4:
        if(0==get_marks(motor_res[_motor_num].pin))
        {
            int pulse = get_pulse(motor_res[_motor_num].pin);
            int _deg = pulse2deg(pulse);
            if(_deg<(5+5))
            {
                _self = SELE_DONE;
                degree = 0;
                return 0;
            }
            else
            {
                _self = SELE_FAIL;
            }
        }
        break;
    case SELE_STEP6:
        if(0==get_marks(motor_res[_motor_num].pin))
        {
            dir(MOTOR_DIR_REV);
            set_mode(MOTOR_MODE_RUN);
            motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(10)); // 5deg
            _self = SELE_STEP6_AUX;
        }
        break;
    case SELE_STEP6_AUX:
        if(0==get_marks(motor_res[_motor_num].pin))
        {
            dir(MOTOR_DIR_FOR);
            set_mode(MOTOR_MODE_RESET);
            motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(15)); // 5deg
            _self = SELE_STEP7;
        }
        break;
    case SELE_STEP7:
        if(0==get_marks(motor_res[_motor_num].pin))
        {
            int pulse = get_pulse(motor_res[_motor_num].pin);
            int _deg = pulse2deg(pulse);
            qDebug("_deg:%d", _deg);
            if(_deg<(10) && (1==signal_in()))
            {
                _self = SELE_DONE;
                degree = 0;
#if 0
                dir(MOTOR_DIR_FOR);
                set_mode(MOTOR_MODE_RUN);
                motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(0.8)); // 5deg
#else
                if(_tune>=0)
                {
                    dir(MOTOR_DIR_FOR);
                    set_mode(MOTOR_MODE_RUN);
                    motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(_tune)); // 5deg
                }
                else
                {
                    dir(MOTOR_DIR_REV);
                    set_mode(MOTOR_MODE_RUN);
                    motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(-_tune)); // 5deg
                }
#endif
                return 0;
            }
            else
            {
                _self = SELE_FAIL;
            }
        }
        break;

    case SELE_DONE:
        return 0;
    case SELE_FAIL:
        return -2;
    case SELE_NULL:
        return -3;
    default:
        return -1;
        //break;
    }
    return 1;
}





// -----------------------------------
Motor_obj_level::Motor_obj_level(QObject *parent) : Motor_obj(parent)
{
}

void Motor_obj_level::deg_rotate(const int _pwm, const int _degree)
{
#if 0
    //Motor_obj::deg_rotate(_pwm, _degree);
    int pulse=0;
    int _deg=0;
    //qDebug ("\n----------------------------------") ;
    //qDebug ("deg_rotate _pwm:%d _deg:%d", _pwm, _degree) ;
    _deg = _degree-degree;
    //qDebug ("deg_rotate degree:%d _deg:%d", degree, _deg) ;
    dir(MOTOR_DIR_FOR);
    if(_deg>=0)
    {
        //dir(MOTOR_DIR_REV);
        //qDebug ("MOTOR_DIR_REV") ;
    }
    else
    {
        //dir(MOTOR_DIR_FOR);
        _deg = 360+_deg;
        //qDebug ("MOTOR_DIR_FOR") ;
    }
    degree = _degree;
    pulse = deg2pulse(_deg);
    //qDebug ("deg_rotate degree:%d _deg:%d pulse:%d", degree, _deg, pulse) ;
    set_mode(MOTOR_MODE_RUN);
    motorPwmWrite(motor_res[_motor_num].pin, _pwm, pulse);
#else
    Motor_obj::deg_rotate(_pwm, _degree);
#endif
}
#if 0
int Motor_obj_level::self()
{
    //return Motor_obj::self();
    switch (_self) {
    case SELE_START:
        if(1==signal_in())
        {
            _self = SELE_STEP6;
            break;
        }
        dir(MOTOR_DIR_FOR);
        set_mode(MOTOR_MODE_RUN);
        motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(5)); // 5deg
        _self = SELE_STEP1;
        break;
    case SELE_STEP1:
        if(0==get_marks(motor_res[_motor_num].pin))
        {
            dir(MOTOR_DIR_FOR);
            set_mode(MOTOR_MODE_RESET);
            motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(360)); // 360deg
            _self = SELE_STEP6;
        }
        break;
    case SELE_STEP6:
        if(0==get_marks(motor_res[_motor_num].pin))
        {
            dir(MOTOR_DIR_FOR);
            set_mode(MOTOR_MODE_RUN);
            motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(5)); // 5deg
            _self = SELE_STEP6_AUX;
        }
        break;
    case SELE_STEP6_AUX:
        if(0==get_marks(motor_res[_motor_num].pin))
        {
            dir(MOTOR_DIR_FOR);
            set_mode(MOTOR_MODE_RESET);
            motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(360)); // 360deg
            _self = SELE_STEP7;
        }
        break;
    case SELE_STEP7:
        if(0==get_marks(motor_res[_motor_num].pin))
        {
            int pulse = get_pulse(motor_res[_motor_num].pin);
            int _deg = pulse2deg(pulse);
            qDebug("_deg:%d", _deg);
            if(_deg<(10) && (1==signal_in()))
            {
                _self = SELE_DONE;
                degree = 0;
                dir(MOTOR_DIR_FOR);
                set_mode(MOTOR_MODE_RUN);
                motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(0.8)); // 5deg
                return 0;
            }
            else
            {
                _self = SELE_FAIL;
            }
        }
        break;

    case SELE_DONE:
        return 0;
    case SELE_FAIL:
        return -2;
    case SELE_NULL:
        return -3;
    default:
        return -1;
    }
    //QThread::msleep(20);
    return 1;
}
#else
int Motor_obj_level::self()
{
    switch (_self) {
    case SELE_START:
        if(1==signal_in())
        {
            _self = SELE_STEP6;
            break;
        }
        dir(MOTOR_DIR_REV);
        set_mode(MOTOR_MODE_RUN);
        motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(50)); // 50deg
        _self = SELE_STEP1;
        break;
    case SELE_STEP1:
        if(0==get_marks(motor_res[_motor_num].pin))
        {
            dir(MOTOR_DIR_FOR);
            set_mode(MOTOR_MODE_RESET);
            motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(370)); // 370deg
            _self = SELE_STEP6;
        }
        break;
    case SELE_STEP6:
        if(0==get_marks(motor_res[_motor_num].pin))
        {
            dir(MOTOR_DIR_REV);
            set_mode(MOTOR_MODE_RUN);
            motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(10)); // 10deg
            _self = SELE_STEP6_AUX;
        }
        break;
    case SELE_STEP6_AUX:
        if(0==get_marks(motor_res[_motor_num].pin))
        {
            dir(MOTOR_DIR_FOR);
            set_mode(MOTOR_MODE_RESET);
            motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(15)); // 15deg
            _self = SELE_STEP7;
        }
        break;
    case SELE_STEP7:
        if(0==get_marks(motor_res[_motor_num].pin))
        {
            int pulse = get_pulse(motor_res[_motor_num].pin);
            int _deg = pulse2deg(pulse);
            qDebug("_deg:%d", _deg);
            if(_deg<(10) && (1==signal_in()))
            {
                _self = SELE_DONE;
                degree = 0;
#if 0
                dir(MOTOR_DIR_FOR);
                set_mode(MOTOR_MODE_RUN);
                motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(0.8)); // 5deg
#else
                if(_tune>=0)
                {
                    dir(MOTOR_DIR_FOR);
                    set_mode(MOTOR_MODE_RUN);
                    motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(_tune)); // 5deg
                }
                else
                {
                    dir(MOTOR_DIR_REV);
                    set_mode(MOTOR_MODE_RUN);
                    motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(-_tune)); // 5deg
                }
#endif
                return 0;
            }
            else
            {
                _self = SELE_FAIL;
            }
        }
        break;

    case SELE_DONE:
        return 0;
    case SELE_FAIL:
        return -2;
    case SELE_NULL:
        return -3;
    default:
        return -1;
        //break;
    }
    //QThread::msleep(20);
    return 1;
}
#endif



Motor_obj_vertical::Motor_obj_vertical(QObject *parent) : Motor_obj(parent)
{
    ;
}

void Motor_obj_vertical::deg_rotate(const int _pwm, const int _degree)
{
    Motor_obj::deg_rotate(_pwm, _degree);
}

int Motor_obj_vertical::self()
{
    switch (_self) {
    case SELE_START:
        if(1==signal_in())
        {
            _self = SELE_STEP6;
            break;
        }
#if 0
        dir(MOTOR_DIR_FOR);
        set_mode(MOTOR_MODE_RUN);
        motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(5)); // 5deg
#else
        dir(MOTOR_DIR_REV);
        set_mode(MOTOR_MODE_RUN);
        motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(50)); // 50deg
#endif
        _self = SELE_STEP1;
        break;
    case SELE_STEP1:
#if 0
        if(0==get_marks(motor_res[_motor_num].pin))
        {
            dir(MOTOR_DIR_FOR);
            set_mode(MOTOR_MODE_RESET);
            motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(180)); // 180deg
            _self = SELE_STEP2;
        }
#else
        if(0==get_marks(motor_res[_motor_num].pin))
        {
            dir(MOTOR_DIR_FOR);
            set_mode(MOTOR_MODE_RESET);
            motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(370)); // 370deg
            _self = SELE_STEP6;
        }
#endif
        break;
    case SELE_STEP6:
        if(0==get_marks(motor_res[_motor_num].pin))
        {
            dir(MOTOR_DIR_REV);
            set_mode(MOTOR_MODE_RUN);
            motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(10)); // 10deg
            _self = SELE_STEP6_AUX;
        }
        break;
    case SELE_STEP6_AUX:
        if(0==get_marks(motor_res[_motor_num].pin))
        {
            dir(MOTOR_DIR_FOR);
            set_mode(MOTOR_MODE_RESET);
            motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(15)); // 15deg
            _self = SELE_STEP7;
        }
        break;
    case SELE_STEP7:
        if(0==get_marks(motor_res[_motor_num].pin))
        {
            int pulse = get_pulse(motor_res[_motor_num].pin);
            int _deg = pulse2deg(pulse);
            qDebug("_deg:%d", _deg);
            if(_deg<(10) && (1==signal_in()))
            {
                _self = SELE_DONE;
                degree = 0;
#if 0
                dir(MOTOR_DIR_FOR);
                set_mode(MOTOR_MODE_RUN);
                motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(0.8)); // 5deg
#else
                if(_tune>=0)
                {
                    dir(MOTOR_DIR_FOR);
                    set_mode(MOTOR_MODE_RUN);
                    motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(_tune)); // 5deg
                }
                else
                {
                    dir(MOTOR_DIR_REV);
                    set_mode(MOTOR_MODE_RUN);
                    motorPwmWrite(motor_res[_motor_num].pin, 300, deg2pulse(-_tune)); // 5deg
                }
#endif
                return 0;
            }
            else
            {
                _self = SELE_FAIL;
            }
        }
        break;

    case SELE_DONE:
        return 0;
    case SELE_FAIL:
        return -2;
    case SELE_NULL:
        return -3;
    default:
        return -1;
        //break;
    }
    //QThread::msleep(20);
    return 1;
}
