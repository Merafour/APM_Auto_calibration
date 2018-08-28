#ifndef QUADROTOR_H
#define QUADROTOR_H

#include <QObject>

#include "motor_obj.h"

class Quadrotor : public QObject
{
    Q_OBJECT
public:
    explicit Quadrotor(QObject *parent = 0);

public:
    enum Acc_Cail{
        STEP_NULL=0,
        STEP_START,
        STEP_ZERO,
        STEP_FMU,
        STEP_LEVEL,
        STEP_LEFT,
        STEP_RIGHT,
        STEP_UP,
        STEP_DOWN,
        STEP_BACK,
        STEP_DONE,
    };
    void init(void);

    void pilot_level(void)
    {
        motor_vertical.deg0(300);
        while(1)
        {
            if(0==motor_vertical.get_mark()) break;
        }
        motor_level.deg0(300);
        while(1)
        {
            if(0==motor_level.get_mark()) break;
        }
    }
    void pilot_up(void)
    {
        motor_vertical.deg90(300);
        while(1)
        {
            if(0==motor_vertical.get_mark()) break;
        }
        //motor_level.deg90(300);
        motor_level.deg270(300);
        while(1)
        {
            if(0==motor_level.get_mark()) break;
        }
    }
    void pilot_down(void)
    {
        motor_vertical.deg90(300);
        while(1)
        {
            if(0==motor_vertical.get_mark()) break;
        }
        //motor_level.deg270(300);
        motor_level.deg90(300);
        while(1)
        {
            if(0==motor_level.get_mark()) break;
        }
    }
    void pilot_left(void)
    {
        motor_vertical.deg90(300);
        while(1)
        {
            if(0==motor_vertical.get_mark()) break;
        }
        motor_level.deg0(300);
        while(1)
        {
            if(0==motor_level.get_mark()) break;
        }
    }
    void pilot_right(void)
    {
        motor_vertical.deg90(300);
        while(1)
        {
            if(0==motor_vertical.get_mark()) break;
        }
        motor_level.deg180(300);
        while(1)
        {
            if(0==motor_level.get_mark()) break;
        }
    }
    void pilot_back(void)
    {
        motor_vertical.deg180(300);
        while(1)
        {
            if(0==motor_vertical.get_mark()) break;
        }
        //motor_level.deg0(300);
    }

    void update(void);

    Motor_obj* level(void)
    {
        return &motor_level;
    }
    Motor_obj* vertical(void)
    {
        return &motor_vertical;
    }

    int  _self;
    int  _axle;
    int check_l;
    int check_v;
    int signal_l;
    int signal_v;

    enum Acc_Cail acc_cail_step;
signals:

public slots:

private:
#if 0
    Motor_obj motor_level;
    Motor_obj motor_vertical;
#else
    Motor_obj_level motor_level;
    Motor_obj_vertical motor_vertical;
#endif

    void self(void);
    void axle(void);

};

#endif // QUADROTOR_H
