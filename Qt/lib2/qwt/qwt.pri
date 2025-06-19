#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

CONFIG += qwt
DEFINES += QT_DLL QWT_DLL
QT   += svg opengl printsupport concurrent

unix {
    DEPENDPATH  += /usr/include/qwt6/
    INCLUDEPATH = $$DEPENDPATH
    greaterThan(QT_MAJOR_VERSION, 5) {
        LIBS += -lqwt6-qt6
    }
    else {
        LIBS += -lqwt6-qt5
    }
}

win32 {
    # QWT_PATH = "qwt-614"
    # QWT_PRI  = "qwt-614.pri"

    # QWT_PATH = "qwt-615"
    # QWT_PRI  = "qwt-615.pri"

    # QWT_PATH = "qwt-620"
    # QWT_PRI  = "qwt-620.pri"

    QWT_PATH = "qwt-630"
    QWT_PRI  = "qwt-630.pri"

    DEPENDPATH  += $$PWD/$$QWT_PATH/src/
    INCLUDEPATH += $$PWD/$$QWT_PATH/src/

    win32:contains(QT_ARCH, x86_64) {
        message("64-bit build")
        LIBS    += -L$$PWD/lib_64
    }
    win32:contains(QT_ARCH, i386) {
        message("32-bit build")
        LIBS    += -L$$PWD/lib_32
    }

    CONFIG(debug, debug|release) {
        LIBS    += -lqwtd
    } else {
        LIBS    += -lqwt
    }
}
