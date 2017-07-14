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
    HLK-RM04_OpenWRT \
    MTC \
    MTC_runner \
    QTextToSpeech \
    RS232_x2 \
    RS232_5_x2 \
    SBGlass \
    SIM900 \
    Sniffer_PCA10000 \
    Teacher_reserv \
    Tools \
    VRM04

unix {
SUBDIRS += \
    Analyzer
}
