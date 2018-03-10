#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS += \
    Template \
    Template_main \
    Template_old \
    Template_wo_MainBox \

unix {
SUBDIRS += \
    Template_browser \
}
