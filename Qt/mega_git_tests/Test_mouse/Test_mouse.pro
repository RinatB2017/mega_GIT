#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_mouse

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp \
    other.hpp

SOURCES += \
    main.cpp

linux {
    CONFIG  += c++11
    LIBS    += -lX11 -lXtst
}

win32 {
    RC_ICONS = ico/computer.ico
    error (Only linux)
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    images/images.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

#----------------------------------------------
win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)
include (src/test_mouse_mainbox/test_mouse_mainbox.pri)
#----------------------------------------------
VPATH = $$INCLUDEPATH
