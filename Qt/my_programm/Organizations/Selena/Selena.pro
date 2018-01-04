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

contains( DEFINES, Q_WS_LINUX ) {
    SUBDIRS += Test_r4_serial_widget
}
