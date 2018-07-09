#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_HID_device

DEPENDPATH  += \
    $$PWD \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

HEADERS += \
    AD9106_box.hpp \
    defines.hpp \
    version.hpp \
    other.hpp

SOURCES += \
    AD9106_box.cpp \
    main.cpp

FORMS   += AD9106_box.ui

win32 {
    RC_FILE = ico/myapp.rc
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
include (src/AD9106_box/AD9106_box.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}
#----------------------------------------------
VPATH = $$INCLUDEPATH
