#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

# QT += printsupport qml sql

unix {
    LIBS    += -L$$PWD/linux_lib
}
win32 {
    LIBS    += -L$$PWD/win_lib
}

# LIBS    += -lCuteReport
