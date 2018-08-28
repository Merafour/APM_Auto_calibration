#-------------------------------------------------
#
# Project created by QtCreator 2018-08-14T16:23:36
#
#-------------------------------------------------

QT       += core gui

LIBS +=  -lwiringPi -lwiringPiDev -lpthread -lm -lcrypt -lrt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pilot
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    motor.cpp \
    motor_obj.cpp \
    worker.cpp \
    motor_pwm.cpp \
    daemon.cpp \
    motor_manage.cpp \
    cailwindow.cpp \
    gcs_mavlink.cpp \
    backthread.cpp \
    accwindow.cpp \
    quadrotor.cpp

HEADERS  += mainwindow.h \
    motor.h \
    motor_obj.h \
    worker.h \
    motor_pwm.h \
    daemon.h \
    motor_manage.h \
    cailwindow.h \
    gcs_mavlink.h \
    mavlink/v2.0/ardupilotmega/ardupilotmega.h \
    mavlink/v2.0/ardupilotmega/mavlink.h \
    mavlink/v2.0/ardupilotmega/mavlink_msg_ahrs.h \
    mavlink/v2.0/ardupilotmega/mavlink_msg_ahrs2.h \
    mavlink/v2.0/ardupilotmega/mavlink_msg_ap_adc.h \
    mavlink/v2.0/ardupilotmega/mavlink_msg_autopilot_version_request.h \
    mavlink/v2.0/ardupilotmega/mavlink_msg_battery2.h \
    mavlink/v2.0/ardupilotmega/mavlink_msg_compassmot_status.h \
    mavlink/v2.0/ardupilotmega/mavlink_msg_data16.h \
    mavlink/v2.0/ardupilotmega/mavlink_msg_data32.h \
    mavlink/v2.0/ardupilotmega/mavlink_msg_data64.h \
    mavlink/v2.0/ardupilotmega/mavlink_msg_data96.h \
    mavlink/v2.0/ardupilotmega/mavlink_msg_ekf_status_report.h \
    mavlink/v2.0/ardupilotmega/mavlink_msg_fence_fetch_point.h \
    mavlink/v2.0/ardupilotmega/mavlink_msg_fence_point.h \
    mavlink/v2.0/ardupilotmega/mavlink_msg_fence_status.h \
    mavlink/v2.0/ardupilotmega/mavlink_msg_hwstatus.h \
    mavlink/v2.0/ardupilotmega/mavlink_msg_limits_status.h \
    mavlink/v2.0/ardupilotmega/mavlink_msg_meminfo.h \
    mavlink/v2.0/ardupilotmega/mavlink_msg_radio.h \
    mavlink/v2.0/ardupilotmega/mavlink_msg_sensor_offsets.h \
    mavlink/v2.0/ardupilotmega/mavlink_msg_set_mag_offsets.h \
    mavlink/v2.0/ardupilotmega/mavlink_msg_simstate.h \
    mavlink/v2.0/ardupilotmega/mavlink_msg_wind.h \
    mavlink/v2.0/ardupilotmega/testsuite.h \
    mavlink/v2.0/ardupilotmega/version.h \
    mavlink/v2.0/checksum.h \
    mavlink/v2.0/mavlink_conversions.h \
    mavlink/v2.0/mavlink_get_info.h \
    mavlink/v2.0/mavlink_helpers.h \
    mavlink/v2.0/mavlink_sha256.h \
    mavlink/v2.0/mavlink_types.h \
    mavlink/v2.0/protocol.h \
    backthread.h \
    accwindow.h \
    quadrotor.h

FORMS    += mainwindow.ui \
    cailwindow.ui \
    accwindow.ui
