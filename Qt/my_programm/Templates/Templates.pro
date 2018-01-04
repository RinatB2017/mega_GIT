#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS += \
    Template \
    Template_main \
    Template_old \

contains( DEFINES, Q_WS_LINUX ) {
SUBDIRS += \
    Template_browser \
}
