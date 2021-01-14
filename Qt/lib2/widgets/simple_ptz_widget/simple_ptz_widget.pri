#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
	$$PWD/src \
	$$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

QT  +=  multimedia
QT  +=  multimediawidgets

SOURCES	+= simple_ptz_widget.cpp
HEADERS += simple_ptz_widget.hpp
FORMS	+= simple_ptz_widget.ui

include ($$LIB_PATH2/widgets/scan_home_network/scan_home_network.pri)
include ($$LIB_PATH2/widgets/mymediarecorder/mymediarecorder.pri)
