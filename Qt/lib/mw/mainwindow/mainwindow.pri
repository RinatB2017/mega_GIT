#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/mw_images
INCLUDEPATH = $$DEPENDPATH

HEADERS   += mainwindow.hpp
SOURCES   += mainwindow.cpp
RESOURCES += \
    $$PWD/mw_images/mw_images.qrc \
    $$PWD/flags/flags.qrc
