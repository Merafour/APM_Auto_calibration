#ifndef MOTOR_H
#define MOTOR_H

#include <QObject>

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

// wPi
#define MOTER_VERTICAL     1   // moter 1
#define MOTER_VERTICAL_EN  4   // moter 1 enable
#define MOTER_VERTICAL_IN  5   // moter 1 input
#define MOTER_VERTICAL_DIR 6   // moter 1 dir

#define MOTER_LEVEL        26  // moter 2
#define MOTER_LEVEL_EN     27  // moter 2 enable
#define MOTER_LEVEL_IN     28  // moter 2 input
#define MOTER_LEVEL_DIR    29  // moter 2 dir

typedef struct {
    int pin;
    int pin_en;
    int pin_in;
    int pin_dir;
    int pwmRange;
} motor_pin_t;
class motor : public QObject
{
    Q_OBJECT
public:
    explicit motor(QObject *parent = 0);
    int init(void);
    //void motor_test(const int pwm, const int en, const int dir);
    void motor1_en(int en);
    void motor1_dir(int dir);
    void motor1_pwm(int pwm);
    void motor2_en(int en);
    void motor2_dir(int dir);
    void motor2_pwm(int pwm);

signals:

public slots:

private:
    static motor_pin_t motor1;
    static motor_pin_t motor2;
};

#endif // MOTOR_H
