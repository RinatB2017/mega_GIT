#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

unix {
    DEPENDPATH  += /usr/include/qwt6/
    INCLUDEPATH = $$DEPENDPATH
    greaterThan(QT_MAJOR_VERSION, 4) {
        LIBS += -lqwt6-qt5
    }
    else {
        LIBS += -lqwt6-qt4
    }
    QT   *= svg opengl printsupport concurrent
}

win32 {
    QWT_PATH = "qwt-613"
    QWT_PRI  = "qwt-613.pri"

    DEPENDPATH  += $$PWD/$$QWT_PATH/src/
    INCLUDEPATH = $$DEPENDPATH
    include ($$PWD/$$QWT_PATH/$$QWT_PRI)
}
