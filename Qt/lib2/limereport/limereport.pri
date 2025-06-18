#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/include
INCLUDEPATH = $$DEPENDPATH

unix {
    LIBS    += -L$$PWD/linux_lib
}
win32 {
    LIBS    += -L$$PWD/win_lib
}

LIBS    += -llimereport
LIBS    += -lQtZint
