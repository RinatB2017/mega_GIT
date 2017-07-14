DEPENDPATH  += \
    /usr/include/qt5/QtCore/5.7.1/QtCore \
    $$PWD \
    $$PWD/QtSerialBus \
    $$PWD/serialbus
INCLUDEPATH = $$DEPENDPATH

CONFIG  += c++11

HEADERS += \
    $$PWD/serialbus/qcanbusdevice.h \
    $$PWD/serialbus/qcanbusfactory.h \
    $$PWD/serialbus/qcanbusframe.h \
    $$PWD/serialbus/qcanbus.h \
    $$PWD/serialbus/qserialbusglobal.h \
    $$PWD/serialbus/qmodbusserver.h \
    $$PWD/serialbus/qmodbusdevice.h \
    $$PWD/serialbus/qmodbusdataunit.h \
    $$PWD/serialbus/qmodbusclient.h \
    $$PWD/serialbus/qmodbusreply.h \
    $$PWD/serialbus/qmodbusrtuserialmaster.h \
    $$PWD/serialbus/qmodbustcpclient.h \
    $$PWD/serialbus/qmodbustcpserver.h \
    $$PWD/serialbus/qmodbusrtuserialslave.h \
    $$PWD/serialbus/qmodbuspdu.h \
    $$PWD/serialbus/qmodbusdeviceidentification.h

HEADERS += \
    $$PWD/serialbus/qcanbusdevice_p.h \
    $$PWD/serialbus/qmodbusserver_p.h \
    $$PWD/serialbus/qmodbusclient_p.h \
    $$PWD/serialbus/qmodbusdevice_p.h \
    $$PWD/serialbus/qmodbusrtuserialmaster_p.h \
    $$PWD/serialbus/qmodbustcpclient_p.h \
    $$PWD/serialbus/qmodbustcpserver_p.h \
    $$PWD/serialbus/qmodbusrtuserialslave_p.h \
    $$PWD/serialbus/qmodbus_symbols_p.h \
    $$PWD/serialbus/qmodbuscommevent_p.h \
    $$PWD/serialbus/qmodbusadu_p.h \

SOURCES += \
    $$PWD/serialbus/qcanbusdevice.cpp \
    $$PWD/serialbus/qcanbus.cpp \
    $$PWD/serialbus/qcanbusfactory.cpp \
    $$PWD/serialbus/qcanbusframe.cpp \
    $$PWD/serialbus/qmodbusserver.cpp \
    $$PWD/serialbus/qmodbusdevice.cpp \
    $$PWD/serialbus/qmodbusdataunit.cpp \
    $$PWD/serialbus/qmodbusclient.cpp \
    $$PWD/serialbus/qmodbusreply.cpp \
    $$PWD/serialbus/qmodbusrtuserialmaster.cpp \
    $$PWD/serialbus/qmodbustcpclient.cpp \
    $$PWD/serialbus/qmodbustcpserver.cpp \
    $$PWD/serialbus/qmodbusrtuserialslave.cpp \
    $$PWD/serialbus/qmodbuspdu.cpp \
    $$PWD/serialbus/qmodbusdeviceidentification.cpp
