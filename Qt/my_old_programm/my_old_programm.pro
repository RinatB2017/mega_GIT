#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS += \
    ADNS_2610 \
    Bluetooth \
    Creator \
    Dooscape \
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
    Teacher_reserv \
    Tools \

unix {
SUBDIRS += \
    Analyzer
}
