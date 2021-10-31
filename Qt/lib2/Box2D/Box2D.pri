#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH += \
    $$PWD/Box2D
INCLUDEPATH += $$DEPENDPATH

unix {
    LIBS    += -lBox2D
}

win32 {
    use_BOX2D_221 {
        DEFINES += BOX2D_221
        DEPENDPATH  += $$PWD/Box2D_221
        INCLUDEPATH += $$DEPENDPATH
        include ($$PWD/Box2D_221/Box2D.pri)
    }

    use_BOX2D_231 {
        DEFINES += BOX2D_231
        DEPENDPATH  += $$PWD/Box2D_231
        INCLUDEPATH += $$DEPENDPATH
        include ($$PWD/Box2D_231/Box2D.pri)
    }

    use_BOX2D_241 {
        DEFINES += BOX2D_241
        DEPENDPATH  += $$PWD/Box2D_241
        INCLUDEPATH += $$DEPENDPATH
        include ($$PWD/Box2D_241/Box2D.pri)
    }
}
