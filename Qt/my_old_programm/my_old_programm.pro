#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS += \
    ADNS_2610 \
    Bluetooth \
    Ethernet \
    event \
    Games \
    GY_521 \
    Hex_view \
    MTC \
    MTC_runner \
    QTextToSpeech \
    SBGlass \
    SIM900 \
    Sniffer_PCA10000 \
    Tools \

contains( DEFINES, Q_WS_LINUX ) {
SUBDIRS += \
    Creator \
    Dooscape \
    Teacher_reserv \
    Analyzer
}
