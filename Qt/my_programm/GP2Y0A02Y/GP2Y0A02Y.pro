#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = GP2Y0A02Y

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/gp2y0a02y_mainbox \
    $$PWD/src/gp2y0a02y_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += RS232_FIXED_SIZE
#DEFINES += RS232_SEND
#DEFINES += RS232_LOG

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += SHOW_SIZE

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    gp2y0a02y_mainbox.hpp \
    defines.hpp \
    version.hpp \

SOURCES += \
    gp2y0a02y_mainbox.cpp \
    main.cpp

FORMS   += gp2y0a02y_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    images/images.qrc \
    

OTHER_FILES += doc/notebook.txt

#----------------------------------------------
LIB_PATH  = "$$PWD/../../lib"
LIB_PATH2 = "$$PWD/../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/meta/grapherbox.pri)

greaterThan(QT_MAJOR_VERSION, 4) {
    include ($$LIB_PATH2/serial5/serial5.pri)
} esle {
    include ($$LIB_PATH2/serial4/serial4.pri)
}

include (src/serial/serial.pri)
include (src/viewer/viewer.pri)
include (src/grapher/grapher.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}
#----------------------------------------------

VPATH = $$INCLUDEPATH
