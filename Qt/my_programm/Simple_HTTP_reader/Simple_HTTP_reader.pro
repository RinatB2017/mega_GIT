#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Simple_HTTP_reader

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/simple_http_reader_mainbox \
    $$PWD/src/simple_http_reader_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

QT  += network

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_RESIZE
DEFINES += SAVE_WIDGETS_LINEEDIT
DEFINES += SAVE_WIDGETS_SPINBOX
DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp \
    simple_http_reader_mainbox.hpp

SOURCES += \
    simple_http_reader_mainbox.cpp \
    main.cpp

FORMS   += simple_http_reader_mainbox.ui

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

#----------------------------------------------
LIB_PATH  = "../../lib"
LIB_PATH2 = "../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/widgets/ipctrl4/ipctrl4.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}
#----------------------------------------------

VPATH = $$INCLUDEPATH
