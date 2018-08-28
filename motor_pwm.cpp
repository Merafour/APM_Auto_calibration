#include "motor_pwm.h"


/*
 * softPwm.c:
 *	Provide many channels of software driven PWM.
 *	Copyright (c) 2012-2017 Gordon Henderson
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as
 *    published by the Free Software Foundation, either version 3 of the
 *    License, or (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with wiringPi.
 *    If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <pthread.h>

#include "wiringPi.h"
//#include "softPwm.h"


// MAX_PINS:
//	This is more than the number of Pi pins because we can actually softPwm.
//	Once upon a time I let pins on gpio expanders be softPwm'd, but it's really
//	really not a good thing.

#define	MAX_PINS	64

// The PWM Frequency is derived from the "pulse time" below. Essentially,
//	the frequency is a function of the range and this pulse time.
//	The total period will be range * pulse time in µS, so a pulse time
//	of 100 and a range of 100 gives a period of 100 * 100 = 10,000 µS
//	which is a frequency of 100Hz.
//
//	It's possible to get a higher frequency by lowering the pulse time,
//	however CPU uage will skyrocket as wiringPi uses a hard-loop to time
//	periods under 100µS - this is because the Linux timer calls are just
//	not accurate at all, and have an overhead.
//
//	Another way to increase the frequency is to reduce the range - however
//	that reduces the overall output accuracy...

#define	PULSE_TIME	100

static volatile int marks         [MAX_PINS] ;
static volatile int range         [MAX_PINS] ;
static volatile pthread_t threads [MAX_PINS] ;
static volatile enum motor_mode modes [MAX_PINS] ;
static volatile int pulse_counts   [MAX_PINS] ;
static volatile int pulse_stop     [MAX_PINS] ;
static volatile int signal_in     [MAX_PINS] ;
static volatile int signal_in_last [MAX_PINS] ;
static volatile int newPin = -1 ;


/*
 * softPwmThread:
 *	Thread to do the actual PWM output
 *********************************************************************************
 */

void motor_set_mode(const int pin, enum motor_mode _mode)
{
    modes[pin] = _mode;
}
enum motor_mode motor_get_mode(const int pin)
{
    return modes[pin];
}

int get_marks(const int pin)
{
    return marks[pin];
}
int get_pulse(const int pin)
{
    return pulse_stop[pin];
}
int deg2pulse(const int _deg)
{
    int pulse = 0;
    pulse = (_deg*6400)/360;
    return pulse;
}
int pulse2deg(const int pulse)
{
    int _deg = 0;
    _deg = (pulse*360)/6400;
    return _deg;
}
static void *motorPwmThread (void *arg)
{
  int pin, mark, space ;
  motor_pin_t _motor;
  int in;
  int pulse=0;
  struct sched_param param ;

  param.sched_priority = sched_get_priority_max (SCHED_RR) ;
  pthread_setschedparam (pthread_self (), SCHED_RR, &param) ;

  //pin = *((int *)arg) ;
  memcpy(&_motor, arg, sizeof(motor_pin_t));
  pin = _motor.pin;
  in = _motor.pin_in;
  free (arg) ;

  pin    = newPin ;
  newPin = -1 ;

  piHiPri (90) ;
#if 0
  for (;;)
  {
    mark  = marks [pin] ;
    space = range [pin] - mark ;

    if (mark != 0)
      digitalWrite (pin, HIGH) ;
    delayMicroseconds (mark * 100) ;

    if (space != 0)
      digitalWrite (pin, LOW) ;
    delayMicroseconds (space * 100) ;
  }
#endif
  pulse=2;
  for (;;)
  {
    mark  = marks [pin] ;
    space = range [pin] - mark ;

    if(pulse_counts[pin]>deg2pulse(20))
    {
        pulse=1;
    }
    else
    {
        pulse=10;
    }
    if (mark != 0)
    {
        digitalWrite (pin, HIGH) ;
    }
    delayMicroseconds (mark * pulse) ;

    if (space != 0)
    {
        digitalWrite (pin, LOW) ;
    }
    delayMicroseconds (space * pulse) ;
    if(pulse_counts[pin]>0)
    {
        pulse_counts[pin]--;
    }
    if(pulse_counts[pin]<=0)
    {
        pulse_stop[pin] = 0;
        marks [pin] = 0 ;
        modes[pin] = MOTOR_MODE_RESET;
    }
#if 0
    signal_in[pin] = digitalRead(in) ;
    if((MOTOR_MODE_RESET == modes[pin]) && (1==signal_in[pin]))
    {
        pulse_stop[pin] = pulse_counts[pin];
        marks [pin] = 0 ;
        pulse_counts[pin] = 0;
    }
#else
    if((1==digitalRead(in)) && (1==signal_in_last[pin]))
    {
        signal_in[pin]=1;
    }
    else
    {
        signal_in[pin] = 0;
    }
    signal_in_last[pin] = digitalRead(in);
    if((MOTOR_MODE_RESET == modes[pin]) && (1==signal_in[pin]))
    {
        pulse_stop[pin] = pulse_counts[pin];
        marks [pin] = 0 ;
        pulse_counts[pin] = 0;
    }
#endif
  }


  return NULL ;
}

int motor_signal_in(const int pin)
{
    return signal_in[pin];
}

/*
 * softPwmWrite:
 *	Write a PWM value to the given pin
 *********************************************************************************
 */

void motorPwmWrite (int pin, int value, int pulse)
{
  if (pin < MAX_PINS)
  {
    /**/ if (value < 0)
      value = 0 ;
    else if (value > range [pin])
      value = range [pin] ;

    marks [pin] = value ;
    //puse_counts[pin] = 200;
    //puse_counts[pin] = 6400;
    pulse_counts[pin] = pulse;
    pulse_stop[pin] = 0;
  }
}


/*
 * softPwmCreate:
 *	Create a new softPWM thread.
 *********************************************************************************
 */

int motorPwmCreate (const motor_pin_t *_motor, int initialValue)
{
  int res ;
  int pin;
  int pwmRange;
  pthread_t myThread ;
  motor_pin_t *passPin ;

  pin = _motor->pin;
  pwmRange = _motor->pwmRange;
  if (pin >= MAX_PINS)
    return -1 ;

  if (range [pin] != 0)	// Already running on this pin
    return -1 ;

  if (pwmRange <= 0)
    return -1 ;

  passPin = (motor_pin_t *)malloc (sizeof (*passPin)) ;
  if (passPin == NULL)
    return -1 ;

  digitalWrite (pin, LOW) ;
  pinMode      (pin, OUTPUT) ;

  marks [pin] = initialValue ;
  range [pin] = pwmRange ;
  modes [pin] = MOTOR_MODE_NULL;
  pulse_counts[pin] = 0;

  //*passPin = pin ;
  memcpy(passPin, _motor, sizeof(motor_pin_t));
  newPin   = pin ;
  res      = pthread_create (&myThread, NULL, motorPwmThread, (void *)passPin) ;

  while (newPin != -1)
    delay (1) ;

  threads [pin] = myThread ;

  return res ;
}


/*
 * softPwmStop:
 *	Stop an existing softPWM thread
 *********************************************************************************
 */

void motorPwmStop (int pin)
{
  if (pin < MAX_PINS)
  {
    if (range [pin] != 0)
    {
      pthread_cancel (threads [pin]) ;
      pthread_join   (threads [pin], NULL) ;
      range [pin] = 0 ;
      digitalWrite (pin, LOW) ;
    }
  }
}



