#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS += \
    _For_tests \
    auto_inc_version \
    Get_MyIP \
    GP2Y0A02Y \
    Icons \
    NewMoonLight \
    old_programm \
    Organizations \
    MODBUS \
    MyBot \
    QRCodeGenerator \
    PacketSender \
    RS232 \
    RS485 \
    show_films \
    Teacher \
    Teacher_light \
    Templates \
    Test_Game \
    Test_NeuronNet \
    Test_OpenGL \
    Test_draw_line \
    WiFi \
    
lessThan(QT_MAJOR_VERSION, 5) {
    message (QT4)
    SUBDIRS += neural/QNeuralNet
}
