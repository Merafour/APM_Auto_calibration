#include "accwindow.h"
#include "ui_accwindow.h"
#include <QRect>
#include <QDesktopWidget>
#include <QSettings>

#define  Setting_Name "./Setting.ini"
static int tunev, tunel;
AccWindow::AccWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AccWindow)
{
    //QSettings setting("./Setting.ini", QSettings::IniFormat);
    QSettings setting(Setting_Name, QSettings::IniFormat);
    uint32_t count;

    bool ok;
    count = setting.value("count").toInt(&ok);
    qDebug() << "count:" << count;
    setting.setValue("count", QString::number(++count));
    ui->setupUi(this);

    ui->pushButton_Acc->setText(tr("Acc Cail"));
    ui->groupBox_3->setTitle("");
#if 1
    ui->pushButton_cail->setEnabled(false);
    ui->pushButton_cail->setVisible(false);
    ui->pushButton_self->setEnabled(false);
    ui->pushButton_self->setVisible(false);
#endif
    this->adjustSize();
    this->setFixedSize(this->width()*2, this->height());
    this->setWindowFlags(this->windowFlags() &(~Qt::WindowMaximizeButtonHint));
    this->setWindowFlags(this->windowFlags() &(~Qt::WindowMinimizeButtonHint));
    ui->pushButton_Acc->setEnabled(false);
    //this->setWindowFlags(Qt::FramelessWindowHint);

    QRect deskRect = QApplication::desktop()->availableGeometry();
//    setGeometry(deskRect.width()/5, deskRect.height()/5, this->width(),this->height());
    this->move(deskRect.width()/16, deskRect.height()/16);

    tunev = setting.value("tunev").toInt(&ok);
    //tunev = -10;
    qDebug() << "tunev:" << tunev;
    tunel = setting.value("tunel").toInt(&ok);
    qDebug() << "tunel:" << tunel;
    setting.setValue("tunev", QString::number(tunev));
    setting.setValue("tunel", QString::number(tunel));
    _backThread.set_tune(tunev, tunel);

    _backThread.start();
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(timer_update()));
    _timer->start(10);
//    _backThread.start();
}

AccWindow::~AccWindow()
{
    delete ui;
}

void AccWindow::on_pushButton_self_clicked()
{
    _backThread.self_start();
}

void AccWindow::on_pushButton_cail_clicked()
{
    _backThread.axle_test();
}

void AccWindow::on_pushButton_Acc_clicked()
{
#if 0
    _backThread.acc_cail_start();
#else
    backThread::ERR _err;
    _err = _backThread.err_number();
    //if((backThread::ERR::ERR_NULL == _err) || (backThread::ERR::ERR_SUCC == _err))
    if((backThread::ERR::ERR_NULL == _err) || (backThread::ERR::ERR_DONE == _err))
    {
        //ui->pushButton_Acc->setText("Acc Cail");
        _backThread.acc_cail_start();
    }
//    else if(backThread::ERR::ERR_CAIL == _err)
//    {
//        ui->pushButton_Acc->setText("Acc Cail");
//    }
    else
    {
        //ui->pushButton_Acc->setText("STOP");
        _backThread.beep_off();
    }
#endif
}

void AccWindow::timer_update()
{
    int check_l;
    int check_v;
    int signal_l;
    int signal_v;
    int ltn;
    int ltn_;
    int vtn;
    int vtn_;
    int _tl, _tv;
    static int key_last = 1;
    int key=1;
    backThread::ERR _err;
    _backThread.update();
    _backThread.read(check_l, check_v, signal_l, signal_v);
    ui->label_in_m1->setText("level motor input:"+QString::number(signal_l) + " check:"+QString::number(check_l));
    ui->label_in_m2->setText("vertical motor input:"+QString::number(signal_v) + " check:"+QString::number(check_v));
    //ui->label_statue->setText(_backThread.status());
    //ui->label_heart->setText("heartbeat:"+QString::number(_backThread.heart()));
    ui->label_heart->setText("boot time(s):"+QString::number(_backThread.boot_ms()/1000));
    key=_backThread.key();
    if((1==key) && (0==key_last))
    {
        backThread::ERR _err;
        _err = _backThread.err_number();
        if((backThread::ERR::ERR_NULL == _err) || (backThread::ERR::ERR_DONE == _err))
        {
            //ui->pushButton_Acc->setText("Acc Cail");
            _backThread.acc_cail_start();
        }
        else
        {
            //ui->pushButton_Acc->setText("STOP");
            _backThread.beep_off();
        }
    }
    key_last = key;
    _backThread.read_tune(vtn, vtn_, _tv, ltn, ltn_, _tl);
    ui->label_tune->setText("vertical:" + QString::number(vtn)+"(+), "+QString::number(vtn_)+"(-)[" + QString::number(_tv)+"] level:" + \
                            QString::number(ltn)+"(+), "+QString::number(ltn_)+"(-)[" + QString::number(_tl)+"]");
    if(tunev!=_tv)
    {
        tunev = _tv;
        QSettings setting(Setting_Name, QSettings::IniFormat);
        setting.setValue("tunev", QString::number(tunev));
    }
    if(tunel!=_tl)
    {
        tunel = _tl;
        QSettings setting(Setting_Name, QSettings::IniFormat);
        setting.setValue("tunel", QString::number(tunel));
    }
    _err = _backThread.err_number();
    if((backThread::ERR::ERR_NULL == _err) || (backThread::ERR::ERR_DONE == _err))
    {
        ui->pushButton_Acc->setText("Acc Cail");
        _backThread.led(1);
    }
//    else if(backThread::ERR::ERR_CAIL == _err)
//    {
//        ui->pushButton_Acc->setText("Acc Cail");
//    }
//    else
    {
        ui->pushButton_Acc->setText("STOP");
        _backThread.led(0);
    }

    //_backThread.led_fmu(1);
    //_backThread.fmu_exist();
    if((backThread::ERR::ERR_DONE == _err) && _backThread.fmu_exist())
    {
        _backThread.led_fmu(0);  // led on
    }
    else if(backThread::ERR::ERR_DONE == _err)
    {
        _backThread.err_number_clear();
        _backThread.led_fmu(1);  // led off
    }
    else
    {
        _backThread.led_fmu(1);  // led off
    }

    switch (_err) {
    case backThread::ERR::ERR_FAIL:
        ui->label_statue->setText("Calibration FAILED");
        break;
    case backThread::ERR::ERR_FMU:
        ui->label_statue->setText("connect to FMU error");
        break;
    case backThread::ERR::ERR_HEART:
        ui->label_statue->setText("recv heartbeat error");
        break;
    case backThread::ERR::ERR_ZERO:
        ui->label_statue->setText("not back to zero");
        break;
    case backThread::ERR::ERR_STOP:
        ui->label_statue->setText("User stop");
        break;
    case backThread::ERR::ERR_SUCC:
    case backThread::ERR::ERR_DONE:
        ui->label_statue->setText("Calibration successful");
        break;
    case backThread::ERR::ERR_TIMEOUT:
        ui->label_statue->setText("Calibration TimeOut");
        break;
    default:
        ui->label_statue->setText(_backThread.status());
        break;
    }
}
