#include "worker.h"

Worker::Worker(QObject *parent) : QObject(parent)
{

}

void Worker::doWork(const QString &parameter)
{
    int nValue=0;
    (void)parameter;
    while(nValue<100)
    {
        ++nValue;
        qDebug("doWork... %d\r\n", nValue);
        QThread::msleep(50);
    }
}
