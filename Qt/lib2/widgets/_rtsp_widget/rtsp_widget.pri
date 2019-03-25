#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
	$$PWD/src \
	$$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

QT  +=  multimedia
QT  +=  multimediawidgets

SOURCES	+= \
    rtsp_widget.cpp \
    rtsp_dialog.cpp
HEADERS += \
    rtsp_widget.hpp \
    rtsp_dialog.hpp
FORMS	+= \
    rtsp_widget.ui \
    rtsp_dialog.ui
