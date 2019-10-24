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
    Sniffer_PCA10000 \
    Tools \

unix {
SUBDIRS += \
    Creator \
    Teacher_reserv \
    Analyzer
}
