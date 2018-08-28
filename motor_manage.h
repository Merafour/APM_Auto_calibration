#ifndef MOTOR_MANAGE_H
#define MOTOR_MANAGE_H

#include <QObject>

#include "motor_obj.h"


enum pilot_cail{
    PILOT_CAIL_NULL=0,
    PILOT_CAIL_START,
    PILOT_CAIL_LEVEL,
    PILOT_CAIL_UP,
    PILOT_CAIL_DOWN,
    PILOT_CAIL_LEFT,
    PILOT_CAIL_RIGHT,
    PILOT_CAIL_BACK,
    PILOT_CAIL_STEP1,
    PILOT_CAIL_STEP2,
    PILOT_CAIL_STEP3,
    PILOT_CAIL_STEP4,
    PILOT_CAIL_STEP5,
    PILOT_CAIL_STEP6,
    PILOT_CAIL_END,
    PILOT_CAIL_DONE,
    PILOT_CAIL_FAIL,
};

class motor_manage : public QObject
{
    Q_OBJECT
public:
    //explicit motor_manage(QObject *parent = 0);

    explicit motor_manage(Motor_obj &_level, Motor_obj &_vertical, QObject *parent = 0);
#if 0
    void pilot_level(void)
    {
        motor_level.deg0(300);
        motor_vertical.deg0(300);
    }
    void pilot_up(void)
    {
        motor_level.deg270(300);
        motor_vertical.deg90e(300);
    }
    void pilot_down(void)
    {
        motor_level.deg90(300);
        motor_vertical.deg90e(300);
    }
    void pilot_left(void)
    {
        motor_level.deg180(300);
        motor_vertical.deg90e(300);
    }
    void pilot_right(void)
    {
        motor_level.deg0(300);
        motor_vertical.deg90e(300);
    }
    void pilot_back(void)
    {
        motor_level.deg0(300);
        motor_vertical.deg180(300);
    }
#else
    void pilot_level(void)
    {
        motor_vertical.deg0(300);
        while(1)
        {
            if(0==motor_vertical.get_mark()) break;
        }
        motor_level.deg0(300);
    }
    void pilot_up(void)
    {
        motor_vertical.deg90(300);
        while(1)
        {
            if(0==motor_vertical.get_mark()) break;
        }
        motor_level.deg90(300);
    }
    void pilot_down(void)
    {
        motor_vertical.deg90(300);
        while(1)
        {
            if(0==motor_vertical.get_mark()) break;
        }
        //motor_level.deg270(300);
        motor_level.deg90e(300);
    }
    void pilot_left(void)
    {
        motor_vertical.deg90(300);
        while(1)
        {
            if(0==motor_vertical.get_mark()) break;
        }
        motor_level.deg0(300);
    }
    void pilot_right(void)
    {
        motor_vertical.deg90(300);
        while(1)
        {
            if(0==motor_vertical.get_mark()) break;
        }
        motor_level.deg180(300);
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
#endif

    void cail_start(void)
    {
        _cail = PILOT_CAIL_START;
    }
    void cail(void);
    void update(void);
    void acc_cail_start(void);
    enum pilot_cail acc_cail_status(void)
    {
        return _acc_cail;
    }
    void acc_cail_next(void);
    void acc_cail(void);
    void self_start(void)
    {
        _self = 1;
    }
    void self(void);

signals:

public slots:

private:
    Motor_obj &motor_level;
    Motor_obj &motor_vertical;

    enum pilot_cail _cail;
    enum pilot_cail _acc_cail;
    int  _self;
};

#endif // MOTOR_MANAGE_H
