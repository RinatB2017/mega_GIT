#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Tester

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp \
    tester.hpp

SOURCES += \
    tester.cpp \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}

#include ($$LIB_PATH/turbo.pri)
#include (resources.pri)

greaterThan(QT_MAJOR_VERSION, 4) {
    message(Qt5)
    QT += widgets
}

VPATH = $$INCLUDEPATH
