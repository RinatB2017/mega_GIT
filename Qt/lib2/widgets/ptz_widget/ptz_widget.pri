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
    ptz_widget.cpp \
    ptz_dialog.cpp
HEADERS += \
    ptz_widget.hpp \
    ptz_dialog.hpp
FORMS	+= \
    ptz_widget.ui \
    ptz_dialog.ui
