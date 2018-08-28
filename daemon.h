#ifndef DAEMON_H
#define DAEMON_H

#include <QObject>
#include <QThread>
#include <QDebug>

#include "motor_obj.h"

enum daemon_cmd{
    DAEMON_CMD_IN_M1=0,
    DAEMON_CMD_IN_M2,
    DAEMON_CMD_UPDATE,
};

enum daemon_event{
    DAEMON_EVENT_STOP=0,
    DAEMON_EVENT_FORWARD,
    DAEMON_EVENT_REVERSE,
};

class Daemon : public QObject
{
    Q_OBJECT
public:



    explicit Daemon(QObject *parent = 0);

signals:
    void result_signals(const enum daemon_cmd cmd, const int value);

public slots:
    void doWork(const int delay) ;
    void _clicked_slots(const int _num, const int _event);
    void do_operations(const int delay);
    void on_clicked();

private:
    Motor_obj _motor1;
    Motor_obj _motor2;
    Motor_obj *motor;


};

class ControllerDaemon : public QObject
{
    Q_OBJECT
        QThread workerThread;
    public:
    ControllerDaemon() {
        Daemon *worker = new Daemon;
        worker->moveToThread(&workerThread);
        connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, SIGNAL(operate(const int)), worker, SLOT(doWork(int)));
        //connect(this, &ControllerDaemon::_clicked_sig, worker, &Daemon::_clicked_slots);
        connect(this, &ControllerDaemon::operations, worker, &Daemon::do_operations);
        connect(this, SIGNAL(_on_clicked()), worker, SLOT(on_clicked()));
        //connect(worker, &Worker::resultReady, this, &Controller::handleResults);
        connect(worker, &Daemon::result_signals, this, &ControllerDaemon::result_slots);

        workerThread.start();
    }
    ~ControllerDaemon() {
        workerThread.quit();
        workerThread.wait();
    }
    void start(void)
    {
        qDebug() << "Worker Run Thread : " << QThread::currentThreadId();
        emit operate(500);
        //emit operations(500);
    }
    void clicked(const enum motor_num _num, const enum daemon_event _event)
    {
        qDebug("clicked motor%d ... %d\r\n", _num+1, _event);
        //emit _clicked_sig((const int)_num, (const int)_event);
        emit operations(500);
    }
    void user(void)
    {
        qDebug() << "Worker Run Thread : " << QThread::currentThreadId();
        //emit operations(500);
        emit _on_clicked();
    }
public slots:
        void result_slots(const enum daemon_cmd cmd, const int value)
        {
            emit result_signals(cmd, value);
        }
        void handleResults(int value)
        {
            qDebug() << "Worker Run Thread : " << QThread::currentThreadId();
            qDebug() << "handleResults : " << value;
            emit resultReady(value);
        }
signals:
    void operate(const int delay);
    void operations(const int delay);
    void resultReady(int value);
    void result_signals(const enum daemon_cmd cmd, const int value);
    void _clicked_sig(const int _num, const int _event);
    void _on_clicked();
};


#endif // DAEMON_H
