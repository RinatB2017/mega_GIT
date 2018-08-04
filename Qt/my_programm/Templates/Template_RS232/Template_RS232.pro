#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Template_RS232

FOLDER  = _Templates

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/template_rs232_mainbox \
    $$PWD/src/template_rs232_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#QMAKE_CXXFLAGS += -fno-show-column

#DEFINES += RS232_FIXED_SIZE
DEFINES += RS232_SEND
#DEFINES += RS232_LOG

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += NO_RESIZE

#DEFINES += DEMO
#DEFINES += "DEMO_YEAR=2017"
#DEFINES += "DEMO_MONTH=9"
#DEFINES += "DEMO_DAY=9"

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    template_rs232_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    template_rs232_mainbox.cpp \
    main.cpp

FORMS   += template_rs232_mainbox.ui

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
LIB_PATH  = "../../../lib"
LIB_PATH2 = "../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serial5.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}
#----------------------------------------------

VPATH = $$INCLUDEPATH
