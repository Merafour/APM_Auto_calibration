#ifndef MOTOR_OBJ_H
#define MOTOR_OBJ_H

#include <QObject>

#include "motor_pwm.h"

enum motor_num{
    MOTOR_NUM1=0,
    MOTOR_NUM2=1,
    MOTOR_NULL,
};
enum motor_dir{
    MOTOR_DIR_FOR=0,
    MOTOR_DIR_REV=1,
};
enum self_status{
    SELE_NULL=0,
    SELE_START=1,
    SELE_STEP1,
    SELE_STEP2,
    SELE_STEP2_AUX,
    SELE_STEP3,
    SELE_STEP3_AUX,
    SELE_STEP4,
    SELE_STEP4_AUX,
    SELE_STEP5,
    SELE_STEP5_AUX,
    SELE_STEP6,
    SELE_STEP6_AUX,
    SELE_STEP7,
    SELE_STEP7_AUX,
    SELE_STEP8,
    SELE_STEP8_AUX,
    SELE_STEP9,
    SELE_STEP9_AUX,
    SELE_FAIL,
    SELE_DONE,
};


class Motor_obj : public QObject
{
    Q_OBJECT
public:
    explicit Motor_obj(QObject *parent = 0);
    int init(enum motor_num _num);
    void en(int _en);
    void dir(enum motor_dir _dir);
    void pwm(int _pwm);
    void pwm(int _pwm, int pulse);
    void stop(void);
    void deg0(int _pwm)
    {
        deg_rotate(_pwm, 0);
    }
    void deg90(int _pwm)
    {
        deg_rotate(_pwm, 90);
    }
    void deg90e(int _pwm)
    {
        deg_rotate(_pwm, -90);
    }
    void deg180(int _pwm)
    {
        deg_rotate(_pwm, 180);
    }
    void deg270(int _pwm)
    {
        deg_rotate(_pwm, 270);
    }
    int deg2pulse(const float _deg)
    {
        float pulse = 0;
        pulse = (_deg*6400)/360;
        return (int)pulse;
    }
    int pulse2deg(const int pulse)
    {
        int _deg = 0;
        _deg = (pulse*360)/6400;
        return _deg;
    }
    virtual void deg_rotate(const int _pwm,const int _degree);
    void set_mode(enum motor_mode _mode);
    enum motor_mode get_mode(void);
    int get_mark(void);

    int signal_in(void);
    void tune(int &tn, int &tn_, int &_t);
    void set_tune(const int _t)
    {
        _tune = _t;
    }
    static Motor_obj* get_obj(enum motor_num _num)
    {
        if(MOTOR_NUM1==_num) return _obj[MOTOR_NUM1];
        if(MOTOR_NUM2==_num) return _obj[MOTOR_NUM2];
        return NULL;
    }

    void self_start(void)
    {
        _self = SELE_START;
    }
    virtual int self(void);

signals:

public slots:

//private:
protected:
//    int pin;
//    int pin_en;
//    int pin_in;
//    int pin_dir;
    enum motor_num _motor_num;
    static int pwmRange;
    static int setup;
    static Motor_obj *_obj[MOTOR_NULL];
    int degree;
    enum self_status _self;
    int _tune;
    int btn; int btn_;

};

class Motor_obj_level : public Motor_obj
{
    Q_OBJECT
public:
    explicit Motor_obj_level(QObject *parent = 0);
    virtual void deg_rotate(const int _pwm,const int _degree);
    virtual int self(void);

signals:

public slots:

//private:
//protected:

};

class Motor_obj_vertical : public Motor_obj
{
    Q_OBJECT
public:
    explicit Motor_obj_vertical(QObject *parent = 0);
    virtual void deg_rotate(const int _pwm,const int _degree);
    virtual int self(void);

signals:

public slots:

//private:
//protected:

};

#endif // MOTOR_OBJ_H
