#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE  = subdirs
CONFIG   *= ordered

SUBDIRS += \
    Navi_receiver \
    Navi_transmitter \
    Parport \
    Test_MKU5 \
    Tester \

unix {
    SUBDIRS += Test_r4_serial_widget
}
