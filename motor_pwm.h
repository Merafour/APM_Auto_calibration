#ifndef MOTOR_PWM_H
#define MOTOR_PWM_H

typedef struct {
    int pin;
    int pin_en;
    int pin_in;
    int pin_dir;
    int tune;  // +
    int tune_; // -
    int pwmRange;
} motor_pin_t;

enum motor_mode{
    MOTOR_MODE_NULL=0,
    MOTOR_MODE_RESET=1,
    MOTOR_MODE_RUN=2,
};


extern int motorPwmCreate (const motor_pin_t *_motor, int initialValue);
//extern void motorPwmWrite (int pin, int value);
extern void motorPwmWrite (int pin, int value, int pulse);
extern void motorPwmStop (int pin);
extern int motor_signal_in(const int pin);
extern int get_marks(const int pin);
extern int get_pulse(const int pin);

void motor_set_mode(const int pin, enum motor_mode _mode);
enum motor_mode motor_get_mode(const int pin);


#endif // MOTOR_PWM_H
