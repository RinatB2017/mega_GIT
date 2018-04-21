#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS += \
    _For_tests \
    auto_inc_version \
    Check_Network \
    hello_speak \
    Get_MyIP \
    GP2Y0A02Y \
    imageviewer \
    Icons \
    Organizations \
    PelcoD_485 \
    neural/QNeuralNet \
    Labyrinth \
    NewMoonLight \
    MODBUS \
    show_films \
    Templates \
    Test_IPCam \
    WiFi \

unix {
SUBDIRS += \
    MyBot \
    QRCodeGenerator \
    Teacher \
    Teacher_light \
}
    
lessThan(QT_MAJOR_VERSION, 5) {
SUBDIRS += \
    RS232/RS232_4
} else {
SUBDIRS += \
    RS232/RS232_5
}
