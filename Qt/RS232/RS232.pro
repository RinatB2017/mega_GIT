#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS += \
    RS232_5 \
    RS232_5_bridge \
    RS232_5_x2

lessThan(QT_MAJOR_VERSION, 5) {
    message (QT4)
    SUBDIRS += RS232
    SUBDIRS += RS232_x2
}
