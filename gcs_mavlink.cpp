#include "gcs_mavlink.h"

#include  <wiringPi.h>
#include  <wiringSerial.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <QDebug>
#include <QThread>
#include <string.h>
#include <QDateTime>
#include <QFileInfo>
#include <QFile>

#include "mavlink/v2.0/mavlink_helpers.h"

/*
 * bootloader
 */
#define BL_PROTOCOL_VERSION 		5		// The revision of the bootloader protocol
// protocol bytes
#define PROTO_INSYNC				0x12    // 'in sync' byte sent before status
#define PROTO_EOC					0x20    // end of command

// Reply bytes
#define PROTO_OK					0x10    // INSYNC/OK      - 'ok' response
#define PROTO_FAILED				0x11    // INSYNC/FAILED  - 'fail' response
#define PROTO_INVALID				0x13	// INSYNC/INVALID - 'invalid' response for bad commands
#define PROTO_BAD_SILICON_REV 		0x14 	// On the F4 series there is an issue with < Rev 3 silicon
// see https://pixhawk.org/help/errata
// Command bytes
#define PROTO_GET_SYNC				0x21    // NOP for re-establishing sync
#define PROTO_GET_DEVICE			0x22    // get device ID bytes
#define PROTO_CHIP_ERASE			0x23    // erase program area and reset program address
#define PROTO_PROG_MULTI			0x27    // write bytes at program address and increment
#define PROTO_GET_CRC				0x29	// compute & return a CRC
#define PROTO_GET_OTP				0x2a	// read a byte from OTP at the given address
#define PROTO_GET_SN				0x2b    // read a word from UDID area ( Serial)  at the given address
#define PROTO_GET_CHIP				0x2c    // read chip version (MCU IDCODE)
#define PROTO_SET_DELAY				0x2d    // set minimum boot delay
#define PROTO_GET_CHIP_DES			0x2e    // read chip version In ASCII
#define PROTO_BOOT					0x30    // boot the application
#define PROTO_DEBUG					0x31    // emit debug information - format not defined
#define PROTO_SET_BAUD				0x33    // set baud rate on uart

const uint8_t GCS_MAVLINK::msg_id[] = {
    MAVLINK_MSG_ID_HEARTBEAT,
    MAVLINK_MSG_ID_AHRS,
    MAVLINK_MSG_ID_ATTITUDE,
    MAVLINK_MSG_ID_AHRS2,
    MAVLINK_MSG_ID_RAW_IMU,
    MAVLINK_MSG_ID_VFR_HUD,
    MAVLINK_MSG_ID_SYS_STATUS,
    MAVLINK_MSG_ID_POWER_STATUS,
    MAVLINK_MSG_ID_MEMINFO,
    MAVLINK_MSG_ID_MISSION_COUNT,
    MAVLINK_MSG_ID_NAV_CONTROLLER_OUTPUT,
    MAVLINK_MSG_ID_GLOBAL_POSITION_INT,
    MAVLINK_MSG_ID_RC_CHANNELS_RAW,
    MAVLINK_MSG_ID_SERVO_OUTPUT_RAW,
    MAVLINK_MSG_ID_RC_CHANNELS,
    MAVLINK_MSG_ID_HWSTATUS,
    MAVLINK_MSG_ID_SYSTEM_TIME,
    MAVLINK_MSG_ID_EKF_STATUS_REPORT,
    MAVLINK_MSG_ID_VIBRATION,
    MAVLINK_MSG_ID_MISSION_CURRENT,
    MAVLINK_MSG_ID_GPS_RAW_INT,
    MAVLINK_MSG_ID_SENSOR_OFFSETS,
    MAVLINK_MSG_ID_REQUEST_DATA_STREAM,
    MAVLINK_MSG_ID_PARAM_VALUE,
    MAVLINK_MSG_ID_HEARTBEAT
};

GCS_MAVLINK::GCS_MAVLINK(QObject *parent) : QObject(parent)
{
    fmu_fd = 0;
    chan = MAVLINK_COMM_0;
    count = 0;
    sysid = 255;
    compid = 1;
    magic = 254;
    _fmu_path = "/dev/ttyACM";
}

GCS_MAVLINK::~GCS_MAVLINK()
{
    if(fmu_fd>0)
    {
        serialClose(fmu_fd);
    }
}

void GCS_MAVLINK::test()
{
    int fd=0;
    //int count=0;
    int buf[1024];
    int len=0;
    int i;
    qDebug() << "GCS_MAVLINK::test";
    if((fd = serialOpen("/dev/ttyACM0", 57600)) <0)
    {
        qDebug() << "open file!";
        return ;
    }
    while(1)
    {
        memset(buf, 0, sizeof(buf));
        //serialPrintf(fd, "%02X%02X", 0x21, 0x20);
#if 0
        serialPutchar(fd, 0x21);
        serialPutchar(fd, 0x20);
#else
        buf[0] = 0x21;
        buf[1] = 0x20;
        //write(fd, buf, 2);
        write(fd, &buf[0], 1);
        write(fd, &buf[1], 1);
        serialFlush(fd);
#endif
        QThread::msleep(10);
        len = serialDataAvail(fd);
        //qDebug() << "serialDataAvail:" << serialDataAvail(fd);
        for(i=0; i<len; i++)
        {
            buf[i] = serialGetchar(fd);
        }
        qDebug() << "serialDataAvail:" << len;
        for(i=0; i<len; i++)
        {
            qDebug("%02X ", buf[i]);
        }
        QThread::msleep(50);
        count++;
        qDebug() << "count:" << count;
        if(count>(20)) break;
    }
    count = 0;
    serialClose(fd);
}

int GCS_MAVLINK::open_fmu()
{
    //qDebug("GCS_MAVLINK::open_fmu");
#if 0
    return _open_fmu();
#else
    // finalise programming and boot the system
    // command:			BOOT/EOC
    // reply:			INSYNC/OK
    // PROTO_BOOT
    const uint8_t reboot[6]={PROTO_BOOT, PROTO_EOC, PROTO_BOOT, PROTO_EOC, PROTO_BOOT, PROTO_EOC};
    int ret=0;
    ret = _open_fmu();
    if(ret!=0) return ret;
    qDebug("GCS_MAVLINK::open_fmu reboot");
    write_fmu(reboot, sizeof(reboot));
    close_fmu();
    QThread::msleep(1000); // delay 1s
    qDebug("GCS_MAVLINK::open_fmu open again");
    ret = _open_fmu();
    if(ret!=0) return ret;
    return ret;
#endif
}

int GCS_MAVLINK::fmu_exist()
{
    QString file;
    int i=0;
//    qDebug() << "fmu_exist";
    for(i=0; i<100; i++)
    {
#if 0
        QFileInfo* file_info=NULL;
        file = _fmu_path+QString::number(i);
        file_info = new QFileInfo(file);
        //qDebug() << file;
        if(file_info->exists())
        {
            delete file_info;
            qDebug() << "file " +_fmu_path+QString::number(i)+" exist";
            return i;
        }
        delete file_info;
#else
        QFile* _file=NULL;
        file = _fmu_path+QString::number(i);
        _file = new QFile(file);
        if(_file->exists())
        {
            delete _file;
            //qDebug() << "file " +_fmu_path+QString::number(i)+" exist i="+QString::number(i);
            return i;
        }
        delete _file;
#endif
    }
    return -1;
}

int GCS_MAVLINK::_open_fmu()
{
    recv_heart = 0;
    recv_arm = 0;
    time_boot_ms = 0;
    if(fmu_fd>0) return 0;
#if 0
    if((fmu_fd = serialOpen("/dev/ttyACM0", 57600)) <0)
    {
        qDebug() << "open /dev/ttyACM0 file!";
        fmu_fd = 0;
    }
    if(fmu_fd>0) return 0;
    if((fmu_fd = serialOpen("/dev/ttyACM1", 57600)) <0)
    {
        qDebug() << "open /dev/ttyACM1 file!";
        fmu_fd = 0;
    }
    if(fmu_fd>0) return 0;
    if((fmu_fd = serialOpen("/dev/ttyACM2", 57600)) <0)
    {
        qDebug() << "open /dev/ttyACM2 file!";
        fmu_fd = 0;
    }
    if(fmu_fd>0) return 0;
    return -1;
#else
    int fmu = fmu_exist();
    if(fmu>=0)
    {
        QString path = _fmu_path+QString::number(fmu);
        if((fmu_fd = serialOpen(path.toStdString().c_str(), 57600)) <0)
        {
            qDebug() << "open " + path + " file!";
            fmu_fd = 0;
        }
    }
    if(fmu_fd>0) return 0;
    return -1;
#endif
}

void GCS_MAVLINK::close_fmu()
{
    if(fmu_fd>=0)
    {
        serialClose(fmu_fd);
    }
    fmu_fd = -1;
}

void GCS_MAVLINK::data_stream()
{
#if 0
    mavlink_request_data_stream_t _request_data_stream;
    mavlink_message_t msg_send;
    _request_data_stream.target_system = sysid;
    _request_data_stream.target_component = compid;
    _request_data_stream.req_message_rate = 1;
#if 1
    _request_data_stream.start_stop = 1; // start
#else
    _request_data_stream.start_stop = 0; // stop
#endif
    _request_data_stream.req_stream_id = 0; // all
    mavlink_msg_request_data_stream_encode(sysid, compid, &msg_send, &_request_data_stream);
    //write(fmu_fd, &msg_send, sizeof(msg_send));
    write_fmu(&msg_send, sizeof(msg_send));
#endif
    data_stream(1);
}

void GCS_MAVLINK::data_stream(uint16_t _rate)
{
    mavlink_request_data_stream_t _request_data_stream;
    mavlink_message_t msg_send;
    _request_data_stream.target_system = sysid;
    _request_data_stream.target_component = compid;
    _request_data_stream.req_message_rate = _rate;
#if 1
    _request_data_stream.start_stop = 1; // start
#else
    _request_data_stream.start_stop = 0; // stop
#endif
    _request_data_stream.req_stream_id = 0; // all
    mavlink_msg_request_data_stream_encode(sysid, compid, &msg_send, &_request_data_stream);
    //write(fmu_fd, &msg_send, sizeof(msg_send));
    write_fmu(&msg_send, sizeof(msg_send));
}

ssize_t GCS_MAVLINK::write_fmu(const void *__buf, size_t __n)
{
    return write(fmu_fd, __buf, __n);
}
#if 0
void GCS_MAVLINK::update()
{
    int buf[1024*20];
    int len=0;
    int i;
    // receive new packets
    mavlink_message_t msg;
    mavlink_status_t status;
    status.packet_rx_drop_count = 0;

    count = 0;

    //qDebug() << "GCS_MAVLINK::update";
    open_fmu();
    //while(1)
    {
        memset(buf, 0, sizeof(buf));
        QThread::msleep(10);
        len = serialDataAvail(fmu_fd);
        //qDebug() << "serialDataAvail:" << serialDataAvail(fd);
        //qDebug() << "serialDataAvail:" << len;
        for(i=0; i<len; i++)
        {
            //qDebug() << "serialDataAvail:" << len;
            buf[i] = serialGetchar(fmu_fd);
            // Try to get a new message
            if (mavlink_parse_char(chan, buf[i], &msg, &status)) {
                packetReceived(status, msg);
            }
        }
//        qDebug() << "serialDataAvail:" << len;
//        for(i=0; i<len; i++)
//        {
//            qDebug("%02X ", buf[i]);
//        }
        QThread::msleep(50);
        //count++;
        //qDebug() << "count:" << count;
        //if(count>(20)) break;
    }
    //count = 0;
    //serialClose(fmu_fd);
}
#endif
void GCS_MAVLINK::update()
{
    int data;
    int len=0;
    int i;
    // receive new packets
    mavlink_message_t msg;
    mavlink_status_t status;
    status.packet_rx_drop_count = 0;
    //open_fmu();
    len = serialDataAvail(fmu_fd);
    //qDebug() << "serialDataAvail:" << len;
    for(i=0; i<len; i++)
    {
        data = serialGetchar(fmu_fd);
        // Try to get a new message
        if (mavlink_parse_char(chan, data, &msg, &status)) {
            packetReceived(status, msg);
        }
    }
    //QThread::msleep(10);
}

void GCS_MAVLINK::calibration_acc_start()
{
    mavlink_command_long_t _command;
    mavlink_message_t msg_send;
#if 0
    _request_data_stream.target_system = sysid;
    _request_data_stream.target_component = compid;
    _request_data_stream.req_message_rate = 1;
    _request_data_stream.start_stop = 0; // start
    _request_data_stream.req_stream_id = 1;
    mavlink_msg_request_data_stream_encode(sysid, compid, &msg_send, &_request_data_stream);
    //write(fmu_fd, &msg_send, sizeof(msg_send));
    write_fmu(&msg_send, sizeof(msg_send));
#endif
    qDebug("calibration_acc_start");
    _command.target_system = sysid;
    _command.target_component = compid;
    _command.command = MAV_CMD_PREFLIGHT_CALIBRATION;
    _command.param5 = 1.0f;
    mavlink_msg_command_long_encode(sysid, compid, &msg_send, &_command);
    write_fmu(&msg_send, sizeof(msg_send));
}

void GCS_MAVLINK::calibration_acc_next()
{
    mavlink_command_ack_t _ack;
    mavlink_message_t msg_send;
    qDebug("calibration_acc_next");
    _ack.command = 1;
    _ack.result = 1;
    mavlink_msg_command_ack_encode(sysid, compid, &msg_send, &_ack);
    write_fmu(&msg_send, sizeof(msg_send));
}

void GCS_MAVLINK::packetReceived(const mavlink_status_t &status, mavlink_message_t &msg)
{
    // we exclude radio packets to make it possible to use the
    // CLI over the radio
    //qDebug() << "msg.msgid:" << QString::number(msg.msgid);
//    mavlink_request_data_stream_t _request_data_stream;
//    mavlink_message_t msg_send;
    switch(msg.msgid)
    {
    case MAVLINK_MSG_ID_HEARTBEAT:
#if 0
        mavlink_heartbeat_t heartbeat;
        mavlink_msg_heartbeat_decode(&msg, &heartbeat);
        // We keep track of the last time we received a heartbeat from our GCS for failsafe purposes
        qDebug() << "MAVLINK_MSG_ID_HEARTBEAT sysid:" << QString::number(msg.sysid);
        qDebug() << "MAVLINK_MSG_ID_HEARTBEAT compid:" << QString::number(msg.compid);
        qDebug() << "MAVLINK_MSG_ID_HEARTBEAT magic:" << QString::number(msg.magic);
//        if(msg->sysid != copter.g.sysid_my_gcs) break;
//        copter.failsafe.last_heartbeat_ms = AP_HAL::millis();
//        copter.pmTest1++;
#endif
        sysid = msg.sysid;
        compid = msg.compid;
        magic = msg.magic;
        //recv_heart = 1;
        recv_heart++;
        data_stream();

//        _request_data_stream.target_system = sysid;
//        _request_data_stream.target_component = compid;
//        _request_data_stream.req_message_rate = 1;
//#if 0
//        _request_data_stream.start_stop = 1; // start
//#else
//        _request_data_stream.start_stop = 0; // stop
//#endif
//        _request_data_stream.req_stream_id = 1;
//        mavlink_msg_request_data_stream_encode(sysid, compid, &msg_send, &_request_data_stream);
//        //write(fmu_fd, &msg_send, sizeof(msg_send));
//        write_fmu(&msg_send, sizeof(msg_send));
        break;
//    case MAVLINK_MSG_ID_AHRS:
//        mavlink_ahrs_t ahrs;
//        mavlink_msg_ahrs_decode(&msg, &ahrs);
//        qDebug("MAVLINK_MSG_ID_AHRS: weight[%f]  error_rp[%f]  error_yaw[%f]  omegaIx[%f]  omegaIy[%f]  omegaIz[%f]  renorm_val[%f]",  \
               ahrs.accel_weight, ahrs.error_rp, ahrs.error_yaw, ahrs.omegaIx, ahrs.omegaIy, ahrs.omegaIz, ahrs.renorm_val);
//        break;
//    case MAVLINK_MSG_ID_ATTITUDE:
//        mavlink_attitude_t attitude;
//        mavlink_msg_attitude_decode(&msg, &attitude);
//        qDebug("MAVLINK_MSG_ID_ATTITUDE: pitch[%f]  pitchspeed[%f]  roll[%f]  rollspeed[%f]  yaw[%f]  yawspeed[%f]  time_boot_ms[%d]",  \
               attitude.pitch, attitude.pitchspeed, attitude.roll, attitude.rollspeed, attitude.yaw, attitude.yawspeed, attitude.time_boot_ms);
//        break;
//    case MAVLINK_MSG_ID_AHRS2:
//        mavlink_ahrs2_t ahrs2;
//        mavlink_msg_ahrs2_decode(&msg, &ahrs2);
//        qDebug("MAVLINK_MSG_ID_AHRS2: pitch[%f]  roll[%f]  yaw[%f]  altitude[%f]  lat[%d]  lng[%d]",  \
               ahrs2.pitch, ahrs2.roll, ahrs2.yaw, ahrs2.altitude, ahrs2.lat, ahrs2.lng);
//        break;
    case MAVLINK_MSG_ID_COMMAND_LONG:
        break;
    case MAVLINK_MSG_ID_COMMAND_ACK:
        {
            mavlink_command_ack_t _command_ack;
            mavlink_msg_command_ack_decode(&msg, &_command_ack);
            qDebug("_command_ack command:%d, result:%d", _command_ack.command, _command_ack.result);
        }
        break;
    case MAVLINK_MSG_ID_STATUSTEXT:
        {
            mavlink_statustext_t  _statustext;
            mavlink_msg_statustext_decode(&msg, &_statustext);
            if(NULL!=strstr(_statustext.text, "Arm: "))
            {
                recv_arm++;
                break;
            }
            if( (NULL!=strstr(_statustext.text, "Place vehicle ")) || \
                    (NULL!=strstr(_statustext.text, "Calibration successful")) || \
                    (NULL!=strstr(_statustext.text, "Calibration cancelled")) || \
                    (NULL!=strstr(_statustext.text, "Calibration FAILED")) )
            {
                memcpy(text, _statustext.text, sizeof(text));
                //break;
            }
            //qDebug("_statustext: %s", _statustext.text);
            qDebug("_statustext [heart:%3d arm:%3d]: %s", recv_heart, recv_arm, _statustext.text);
        }
        break;
#if 0
    case MAVLINK_MSG_ID_PARAM_VALUE:
        {
            mavlink_param_value_t _param_value;
            mavlink_msg_param_value_decode(&msg, &_param_value);
            qDebug("_param_value: %s", _param_value.param_id);
        }
        break;
#endif
#if 0
    case MAVLINK_MSG_ID_SYS_STATUS:
        {
            mavlink_sys_status_t _sys_status;
            mavlink_msg_sys_status_decode(&msg, &_sys_status);
            qDebug("_sys_status: load[%d] sensors_enabled[%08X] sensors_health[%08X] sensors_present[%08X]", _sys_status.load, _sys_status.onboard_control_sensors_enabled, \
                   _sys_status.onboard_control_sensors_health, _sys_status.onboard_control_sensors_present);
        }
        break;
#endif
    case MAVLINK_MSG_ID_SYSTEM_TIME:
        {
            mavlink_system_time_t _system_time;
            mavlink_msg_system_time_decode(&msg, &_system_time);
            time_boot_ms = _system_time.time_boot_ms;
            //qDebug("_system_time: time_boot_ms[%d] time_unix_usec[%d] ", _system_time.time_boot_ms, _system_time.time_unix_usec);
        }
        break;
    default:
        //qDebug() << "msg.msgid:" << QString::number(msg.msgid);
        {
#if 1
            int i=0;
            for(i=0; i<(sizeof(msg_id)/sizeof(msg_id[0])); i++)
            {
                if(msg.msgid == msg_id[i]) break;
            }
            if(i==(sizeof(msg_id)/sizeof(msg_id[0])))
                qDebug() << "msg.msgid:" << QString::number(msg.msgid);
#else
                qDebug() << "msg.msgid:" << QString::number(msg.msgid);
#endif
        }
        break;
    }
//    if (msg.msgid != MAVLINK_MSG_ID_RADIO && msg.msgid != MAVLINK_MSG_ID_RADIO_STATUS) {
//        mavlink_active |= (1U<<(chan-MAVLINK_COMM_0));
//    }
//    if (!(status.flags & MAVLINK_STATUS_FLAG_IN_MAVLINK1) &&
//        (status.flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) &&
//        serialmanager_p &&
//        serialmanager_p->get_mavlink_protocol(chan) == AP_SerialManager::SerialProtocol_MAVLink2) {
//        // if we receive any MAVLink2 packets on a connection
//        // currently sending MAVLink1 then switch to sending
//        // MAVLink2
//        mavlink_status_t *cstatus = mavlink_get_channel_status(chan);
//        if (cstatus != NULL) {
//            cstatus->flags &= ~MAVLINK_STATUS_FLAG_OUT_MAVLINK1;
//        }
//    }
//    // if a snoop handler has been setup then use it
//    if (msg_snoop != NULL) {
//        msg_snoop(&msg);
//    }
//    if (routing.check_and_forward(chan, &msg)) {
//        handleMessage(&msg);
//    }
}




















