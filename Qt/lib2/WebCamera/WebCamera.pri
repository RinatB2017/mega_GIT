#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui \
    $$PWD/xml
INCLUDEPATH = $$DEPENDPATH

QT  += opengl

HEADERS += webcamera.hpp
SOURCES += webcamera.cpp
FORMS   += webcamera.ui

HEADERS += cqtopencvviewergl.h
SOURCES += cqtopencvviewergl.cpp

include ($$LIB_PATH2/opencv/opencv.pri)
