#ifndef GCS_MAVLINK_H
#define GCS_MAVLINK_H

#include <QObject>
#include <string.h>
#include <QString>


#include "mavlink/v2.0/mavlink_types.h"

class GCS_MAVLINK : public QObject
{
    Q_OBJECT
public:
    explicit GCS_MAVLINK(QObject *parent = 0);
    ~GCS_MAVLINK() ;
//    {
//        if(fmu_fd>0)
//        {
//            serialClose(fmu_fd);
//        }
//    }

    void test(void);

    void update(void);
    void calibration_acc_start(void);
    void calibration_acc_next(void);
    int get_text(char* buf)
    {
        memcpy(buf, text, sizeof(text));
        return 0;
    }
    const char* _text(void)
    {
        return text;
    }
    void clear_text(void)
    {
       memset(text, 0, sizeof(text));
    }
    int open_fmu(void);
    int fmu_exist(void);

    void close_fmu(void);
    void data_stream(void);
    void data_stream(uint16_t _rate);

    int recv_heart;
    int recv_arm;
    uint32_t time_boot_ms; /*< Timestamp of the component clock since boot time in milliseconds.*/


signals:

public slots:

protected:
    void packetReceived(const mavlink_status_t &status, mavlink_message_t &msg);
    ssize_t write_fmu (const void *__buf, size_t __n);

private:
    int _open_fmu(void);
    QString _fmu_path;

    int count;
    int fmu_fd;
    mavlink_channel_t           chan;

    uint8_t magic;          ///< protocol magic marker
    uint8_t sysid;          ///< ID of message sender system/aircraft
    uint8_t compid;         ///< ID of the message sender component

    static const uint8_t msg_id[];
    char text[50]; /*< Status text message, without null termination character*/
};

#endif // GCS_MAVLINK_H
