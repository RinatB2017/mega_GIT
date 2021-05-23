#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS += \
    $$PWD/ADNS_2610 \
    $$PWD/Bluetooth \
    $$PWD/Ethernet \
    $$PWD/Games \
    $$PWD/GY_521 \
    $$PWD/Hex_view \
    $$PWD/MTC \
    $$PWD/MTC_runner \
    $$PWD/QTextToSpeech \
    $$PWD/SBGlass \
    $$PWD/Sniffer_PCA10000 \
    $$PWD/Tools

unix {
SUBDIRS += \
    $$PWD/Creator \
    $$PWD/Teacher_reserv \
    $$PWD/Analyzer
}
