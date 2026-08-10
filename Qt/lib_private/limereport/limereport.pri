#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

QT += printsupport qml sql

unix {
    DEPENDPATH  += \
        $$PWD/linux_lib/include
    INCLUDEPATH = $$DEPENDPATH

    LIBS    += -L$$PWD/linux_lib
}
win32 {
    DEPENDPATH  += \
        $$PWD/win_lib/include
    INCLUDEPATH = $$DEPENDPATH

    LIBS    += -L$$PWD/win_lib
}

LIBS    += -llimereport
LIBS    += -lQtZint
