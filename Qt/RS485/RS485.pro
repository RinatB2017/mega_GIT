#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE  = subdirs
CONFIG   += ordered

greaterThan(QT_MAJOR_VERSION, 4) {
    SUBDIRS += \
        RS485 \
        RS485_test
}
