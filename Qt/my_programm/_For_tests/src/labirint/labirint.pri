#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    cell.hpp \
    world.hpp \
    minimap.hpp \
    labirint.hpp

SOURCES += \
    cell.cpp \
    world.cpp \
    minimap.cpp \
    labirint.cpp

FORMS += labirint.ui
