#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = RS232_5_bridge

include (conf/conf.pri)

FOLDER      = RS232

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/RS232.ico
}

win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)

include (src/rs232_5_bridge_mainbox/rs232_5_bridge_mainbox.pri)
#----------------------------------------------
CONFIG  += use_worker
use_worker {
    DEFINES += FAKE
    include ($$LIB_PATH2/serial5/fake_serialwidget/serialwidget/serialwidget.pri)
    include (src/worker_fake/worker_fake.pri)
} else {
    include ($$LIB_PATH2/serial5/serialwidget/serialwidget.pri)
}
include ($$LIB_PATH2/serial5/serial5.pri)
#----------------------------------------------

lessThan(QT_MAJOR_VERSION, 5) {
    error (Only Qt5)
}

RESOURCES += \
    ico/icons.qrc

VPATH = $$INCLUDEPATH


