#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

unix {
    LIBS    += -lbox2d
}

win32 {
    include ($$PWD/win/Box2D.pri)
}
