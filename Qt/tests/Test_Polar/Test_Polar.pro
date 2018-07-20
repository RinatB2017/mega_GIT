#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_Polar

FOLDER  = tests

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/test_polar \
    $$PWD/src/test_polar/ui
INCLUDEPATH = $$DEPENDPATH

#QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += NO_RESIZE

DEFINES += SAVE_WIDGETS_CHECKBOX
DEFINES += SAVE_WIDGETS_SPINBOX

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    test_polar.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_polar.cpp \
    main.cpp

FORMS   += test_polar.ui

win32 {
    RC_FILE = ico/myapp.rc
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    images/images.qrc \
    doc/doc.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

LIB_PATH  = "../../../Qt/lib"
LIB_PATH2 = "../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH/mywidgets/myfiledialog.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
