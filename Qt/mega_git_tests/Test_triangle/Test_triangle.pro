#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_triangle

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_triangle_mainbox \
    $$PWD/src/test_triangle_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    test_triangle_mainbox.hpp \
    defines.hpp

SOURCES += \
    test_triangle_mainbox.cpp \
    main.cpp

FORMS   += test_triangle_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)

include ($$LIB_PATH/meta/mainwindow.pri)

VPATH = $$INCLUDEPATH
