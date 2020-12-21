#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_HTML

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QT += webenginewidgets
CONFIG += c++11

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += SAVE_INI

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    images/images.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH/mywidgets/filemanager/filemanager.pri)
include ($$LIB_PATH2/highlighter/highlighter.pri)
include ($$LIB_PATH2/codeeditor/codeeditor.pri)

include (src/test_html_mainbox/test_html_mainbox.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = $$PWD/src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
