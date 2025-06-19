#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/QtRPT \
    $$PWD/CommonFiles
INCLUDEPATH = $$DEPENDPATH

include ($$PWD/QtRPT/QtRPT.pri)

unix {
    LIBS    += -L$$PWD/linux_lib
    LIBS    += -lQtZint
}
win32 {
    LIBS    += -L$$PWD/win_lib
    LIBS    += -lQtZint
}
