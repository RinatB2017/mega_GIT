#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/QtRPT
INCLUDEPATH = $$DEPENDPATH

QT  += printsupport qml xml sql

unix {
    LIBS    += -L$$PWD/linux_lib
    LIBS    += -lQtZint
    LIBS    += -lQtRPT
}

win32 {
    LIBS    += -L$$PWD/win_lib
    LIBS    += -lQtRPT
}
