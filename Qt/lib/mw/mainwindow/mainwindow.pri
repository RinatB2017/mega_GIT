#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/mainwindow \
    $$PWD/mw_images
INCLUDEPATH = $$DEPENDPATH

include (creatorwindow/creatorwindow.pri)
include (creatortoolbars/creatortoolbars.pri)
include (creatormenus/creatormenus.pri)

HEADERS   += mainwindow.hpp
SOURCES   += mainwindow.cpp
RESOURCES += \
    $$PWD/mw_images/mw_images.qrc \
    $$PWD/flags/flags.qrc
