#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_mouse

FOLDER  = tests

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/test_mouse \
    $$PWD/src/test_mouse/ui
INCLUDEPATH = $$DEPENDPATH



DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

HEADERS += \
    test_mouse.hpp \
    defines.hpp \
    version.hpp \
    other.hpp

SOURCES += \
    test_mouse.cpp \
    main.cpp

FORMS   += test_mouse.ui

CONFIG  += c++11
LIBS    += -lX11 -lXtst

win32 {
    RC_ICONS = ico/computer.ico
    error (Only linux)
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    images/images.qrc \
    doc/doc.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

#----------------------------------------------
LIB_PATH  = "../../lib"
LIB_PATH2 = "../../lib2"
include ($$LIB_PATH/meta/mainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}
#----------------------------------------------
VPATH = $$INCLUDEPATH
