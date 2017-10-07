#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS += \
    _For_tests \
    auto_inc_version \
    Icons \
    old_programm \
    Organizations \
    MODBUS \
    QRCodeGenerator \
    PacketSender \
    RS232 \
    RS485 \
    show_films \
    Teacher \
    Teacher_light \
    Templates \
    Test_NewMoonLight
    
lessThan(QT_MAJOR_VERSION, 5) {
    message (QT4)
    SUBDIRS += neural/QNeuralNet
}
