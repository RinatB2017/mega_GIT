#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    commands.h \
    qhexedit.h \
    qhexedit_p.h \
    xbytearray.h \
    doublehexeditor.hpp
    
SOURCES += \
    commands.cpp \
    qhexedit.cpp \
    qhexedit_p.cpp \
    xbytearray.cpp \
    doublehexeditor.cpp
    
OTHER_FILES += license.txt
