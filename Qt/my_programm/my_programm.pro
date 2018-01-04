#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS += \
    _For_tests \
    auto_inc_version \
    hello_speak \
    Get_MyIP \
    GP2Y0A02Y \
    imageviewer \
    Icons \
    NewMoonLight \
    Organizations \
    MODBUS \
    RS232 \
    RS485 \
    show_films \
    Templates \
    WiFi \

contains( DEFINES, Q_WS_LINUX ) {
    SUBDIRS += \
        MyBot \
        QRCodeGenerator \
        Teacher \
        Teacher_light \
}
    
lessThan(QT_MAJOR_VERSION, 5) {
    message (QT4)
    SUBDIRS += neural/QNeuralNet
}
