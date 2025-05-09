#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_SerialBox5

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

DEFINES += RS232_SEND

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

OTHER_FILES += \
    doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

win32 {
    RC_ICONS = ico/computer.ico
}

RESOURCES += \
    ico/icons.qrc

win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)
#----------------------------------------------
#CONFIG  += use_worker
use_worker {
#    DEFINES += FAKE
    include ($$LIB_PATH2/serial5/fake_serialwidget/serialwidget/serialwidget.pri)
#    include (src/serialwidget_fake/serialwidget_fake.pri)
    include (src/worker_fake/worker_fake.pri)
} else {
    include ($$LIB_PATH2/serial5/serialwidget/serialwidget.pri)
}
include ($$LIB_PATH2/serial5/serial5.pri)
#----------------------------------------------

include (src/test_SerialBox5_mainbox/test_SerialBox5_mainbox.pri)
#----------------------------------------------

VPATH = $$INCLUDEPATH
