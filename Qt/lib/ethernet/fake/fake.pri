#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD
INCLUDEPATH = $$DEPENDPATH

QT -= network

SOURCES += \
    qudpsocket.cpp \
    qtcpsocket.cpp \
    qhostaddress.cpp \
    qabstractsocket.cpp \
    qnetworkinterface.cpp

HEADERS += \
    QUdpSocket \
    QTcpSocket \
    QHostAddress \
    QAbstractSocket \
    QNetworkInterface

HEADERS += \
    qudpsocket.h \
    qtcpsocket.h \
    qhostaddress.h \
    qabstractsocket.h \
    qnetworkinterface.h
    
VPATH = $$INCLUDEPATH
