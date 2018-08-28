#ifndef BACKTHREAD_H
#define BACKTHREAD_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QString>


class backThread : public QObject
{
    Q_OBJECT
public:
    explicit backThread(QObject *parent = 0);
    ~backThread();
public:
    enum ERR{
        ERR_NULL=0,
        ERR_CAIL,
        ERR_SUCC,
        ERR_DONE,
        ERR_FMU,
        ERR_HEART,
        ERR_FAIL,
        ERR_TIMEOUT,
        ERR_STOP,
        ERR_ZERO,
    };
    void start(void);
    void update(void);
    static void beep_on(void);
    static void beep_off(void);
    static int key(void);
    static int led(int flag);
    static int led_fmu(int flag);
    static int heart(void);
    static uint32_t boot_ms(void);
    static int fmu_exist(void);

    void self_start(void);
    void axle_test(void);
    void acc_cail_start(void);
    //int self(void);
    //int cail(void);
    void read(int &check_l, int &check_v, int &signal_l, int &signal_v);
    void read_tune(int &vtn, int &vtn_, int &_tv, int &ltn, int &ltn_, int &_tl);
    void set_tune(const int _tv, const int _tl);


    QString status(void);
    enum ERR err_number(void);
    void err_number_clear(void);

signals:

public slots:

private:
    pthread_t myThread;
};

#endif // BACKTHREAD_H
