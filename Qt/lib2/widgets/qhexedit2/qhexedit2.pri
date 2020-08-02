#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

DEFINES += QHEXEDIT_EXPORTS

HEADERS += \
    chunks.hpp \
    commands.hpp \
    qhexedit.hpp \
    qhexedit2.hpp

SOURCES += \
    chunks.cpp \
    commands.cpp \
    qhexedit.cpp \
    qhexedit2.cpp 
