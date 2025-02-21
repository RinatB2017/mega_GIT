#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_PostgreSQL

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_PostgreSQL_mainbox \
    $$PWD/src/test_PostgreSQL_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QT  += sql

HEADERS += \
    test_PostgreSQL_mainbox.hpp \
    defines.hpp

SOURCES += \
    test_PostgreSQL_mainbox.cpp \
    main.cpp

FORMS   += test_PostgreSQL_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)

VPATH = $$INCLUDEPATH
