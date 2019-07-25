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
    Get_acceleration_data \
    Get_MyIP \
    GP2Y0A02Y \
    imageviewer \
    Icons \
    PelcoD_485 \
    Reader_SAAT_T505 \
    Simple_HTTP_reader \
    neural/QNeuralNet \
    Labyrinth \
    MODBUS \
    show_films \
    Templates \
    WiFi \

unix {
SUBDIRS += \
    MyBot \
    QRCodeGenerator \
    Teacher \
    Teacher_light \
    Walkers \
}
    
lessThan(QT_MAJOR_VERSION, 5) {
SUBDIRS += \
    RS232/RS232_4
} else {
SUBDIRS += \
    RS232/RS232_5
}
