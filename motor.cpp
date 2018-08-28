#include "motor.h"
#include <wiringPi.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <QDebug>

motor_pin_t motor::motor1 = {
    .pin = MOTER_VERTICAL,
    .pin_en = MOTER_VERTICAL_EN,
    .pin_in = MOTER_VERTICAL_IN,
    .pin_dir = MOTER_VERTICAL_DIR,
    .pwmRange = 500,
};
motor_pin_t motor::motor2 = {
    .pin = MOTER_LEVEL,
    .pin_en = MOTER_LEVEL_EN,
    .pin_in = MOTER_LEVEL_IN,
    .pin_dir = MOTER_LEVEL_DIR,
    .pwmRange = 500,
};

motor::motor(QObject *parent) : QObject(parent)
{

}

int motor::init()
{
    qDebug ("motor::init wiringPiSetup\n") ;

    if (wiringPiSetup () == -1)
      exit (1) ;
    qDebug ("motor::init()\n") ;
    softPwmCreate(motor1.pin, 0, motor1.pwmRange);  //创建软PWM
    pinMode (motor1.pin_en, OUTPUT) ;
    digitalWrite (motor1.pin_en, LOW) ;	// Off
    //digitalWrite (motor1.pin_en, HIGH) ;	// On
    pinMode (motor1.pin_dir, OUTPUT) ;
    //digitalWrite (motor1.pin_dir, LOW) ;	// Off
    digitalWrite (motor1.pin_dir, HIGH) ;	// On
    pinMode (motor1.pin_in, INPUT) ;	//delayMicroseconds (1) ;
    softPwmCreate(motor2.pin, 0, motor2.pwmRange);  //创建软PWM
    pinMode (motor2.pin_en, OUTPUT) ;
    digitalWrite (motor2.pin_en, LOW) ;	// Off
    pinMode (motor2.pin_in, INPUT) ;
    pinMode (motor2.pin_dir, OUTPUT) ;
    //digitalWrite (motor2.pin_dir, LOW) ;	// Off
    digitalWrite (motor2.pin_dir, HIGH) ;	// On
    delayMicroseconds (1) ;
    return 0;
}

//void motor::motor_test(const int pwm, const int en, const int dir)
//{
//    //digitalWrite (motor1.pin_en, en) ;	// On
//    //digitalWrite (motor1.pin_dir, dir) ;	// On
//    softPwmWrite(motor1.pin, pwm);
//}

void motor::motor1_en(int en)
{
    en = ~en;
    en = en&0x1;
    digitalWrite (motor1.pin_en, en) ;	// On
}

void motor::motor1_dir(int dir)
{
    digitalWrite (motor1.pin_dir, dir) ;	// On
}

void motor::motor1_pwm(int pwm)
{
    softPwmWrite(motor1.pin, pwm);
}

void motor::motor2_en(int en)
{
    en = ~en;
    en = en&0x1;
    digitalWrite (motor2.pin_en, en) ;	// On
}

void motor::motor2_dir(int dir)
{
    digitalWrite (motor2.pin_dir, dir) ;	// On
}

void motor::motor2_pwm(int pwm)
{
    softPwmWrite(motor2.pin, pwm);
}

