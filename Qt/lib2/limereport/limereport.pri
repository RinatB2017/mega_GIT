#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

unix {
    DEPENDPATH  += \
        $$PWD/linux_lib/include
    INCLUDEPATH = $$DEPENDPATH

    LIBS    += -L$$PWD/linux_lib
    LIBS    += -llimereport
    LIBS    += -lQtZint
}
win32 {
    DEPENDPATH  += \
        $$PWD/win_lib/include
    INCLUDEPATH = $$DEPENDPATH

    QT += printsupport qml
    LIBS    += -L$$PWD/win_lib
    LIBS    += -llimereport
    LIBS    += -lQtZint
}
