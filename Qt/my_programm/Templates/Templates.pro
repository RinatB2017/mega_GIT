#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS += \
    Template \
    Template_main \
    Template_old \

unix {
SUBDIRS += \
    Template_browser \
}
