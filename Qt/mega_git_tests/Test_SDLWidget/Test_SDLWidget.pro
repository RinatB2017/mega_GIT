#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_SDLWidget

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

RESOURCES   += image/image.qrc
OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/SDLWidget/SDLWidget.pri)

VPATH = $$INCLUDEPATH
