#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_Trader

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/trader \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui \
    $$PWD/src/mainbox/
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

#DEFINES += FIXED_SIZE
#DEFINES += FLAG_RESIZE
#
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
#DEFINES += NO_TOOLBAR
#DEFINES += NO_LOG

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    plot.h \
    quotefactory.h \
    griditem.h \
    legend.h

SOURCES += \
    griditem.cpp \
    legend.cpp \
    quotefactory.cpp \
    plot.cpp

HEADERS += \
    traderplot.hpp \
    defines.hpp \
    version.hpp \
    mainbox.hpp

SOURCES += \
    traderplot.cpp \
    mainbox.cpp \
    main.cpp

FORMS += mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_FILE = ico/myapp.rc
}


LIB_PATH = "../../Qt/lib"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH/meta/grapherbox.pri)
include ($$LIB_PATH/serial/serial.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
