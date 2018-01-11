#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Analyzer

FOLDER  = old_programm

DEPENDPATH  += \
    /usr/include/libftdi1 \
    $$PWD/src \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui \
    $$PWD/src/ft2232h_thread \
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

HEADERS += \
    defines.hpp \
    version.hpp \
    mainbox.hpp \
    ft2232h_thread.hpp

SOURCES += \
    mainbox.cpp \
    main.cpp \
    ft2232h_thread.cpp

FORMS += mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_FILE = ico/myapp.rc
}

unix {
    LIBS    += -lftdi1
}

LIBS    += -lusb
LIBS    += /opt/lib64/libftd2xx.so

LIB_PATH = "../../lib"

include ($$LIB_PATH/GrapherBox/GrapherBox.pri)
include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH/csvreader/csvreader.pri)
include ($$LIB_PATH/qwt/qwt.pri)
include ($$LIB_PATH/time/time.pri)

win32 {
    include ($$LIB_PATH/ftdi/ftdi.pri)
}

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
