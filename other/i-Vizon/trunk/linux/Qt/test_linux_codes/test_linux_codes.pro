TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

IVIZON_TRUNK    = /opt/i-vizon/trunk
MESSAGE_QUEUES = $$IVIZON_TRUNK/linux/msg_q

SOURCES += $$MESSAGE_QUEUES/src/msg_q_linux_wrapper.cpp \
           $$MESSAGE_QUEUES/test_wrapper/sendNReceive.cpp

HEADERS += $$MESSAGE_QUEUES/inc/msg_q_linux_wrapper.h
