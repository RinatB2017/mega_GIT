#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = GP2Y0A02Y

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src 
INCLUDEPATH = $$DEPENDPATH

DEFINES += RS232_FIXED_SIZE
#DEFINES += RS232_SEND
#DEFINES += RS232_LOG

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    images/images.qrc

OTHER_FILES += doc/notebook.txt

#----------------------------------------------
LIB_PATH  = "$$PWD/../../lib"
LIB_PATH2 = "$$PWD/../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/meta/grapherbox.pri)

greaterThan(QT_MAJOR_VERSION, 4) {
    include ($$LIB_PATH2/serial5/serialwidget/serialwidget.pri)
    include ($$LIB_PATH2/serial5/serial5.pri)
} esle {
    include ($$LIB_PATH2/serial4/serial4.pri)
}

include (src/serial/serial.pri)
include (src/viewer/viewer.pri)
include (src/grapher/grapher.pri)

include (src/gp2y0a02y_mainbox/gp2y0a02y_mainbox.pri)
#----------------------------------------------

VPATH = $$INCLUDEPATH
