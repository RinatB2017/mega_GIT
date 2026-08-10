#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    stable.h \
    wwglobal.h \
    wwglobal_p.h \
    colormodel.h

SOURCES += \
    colormodel.cpp \
    wwglobal_p.cpp

OTHER_FILES += swap.xpm
