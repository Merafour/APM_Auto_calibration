#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QDebug>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);

public slots:
    void doWork(const QString &parameter) ;

signals:

//public slots:
};

class Controller : public QObject
{
    Q_OBJECT
        QThread workerThread;
    public:
    Controller() {
        Worker *worker = new Worker;
        worker->moveToThread(&workerThread);
        connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &Controller::operate, worker, &Worker::doWork);
        //connect(worker, &Worker::resultReady, this, &Controller::handleResults);
        workerThread.start();
    }
    ~Controller() {
        workerThread.quit();
        workerThread.wait();
    }
    void start(const QString &COM)
    {
        qDebug() << "Worker Run Thread : " << QThread::currentThreadId();
        operate(COM);
    }
    public slots:
        void handleResults(int value)
        {
            qDebug() << "Worker Run Thread : " << QThread::currentThreadId();
            qDebug() << "handleResults : " << value;
            emit resultReady(value);
        }
signals:
    void operate(const QString &);
    void resultReady(int value);
};

#endif // WORKER_H
