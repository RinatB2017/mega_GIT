#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_syslog

FOLDER  = tests

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/test_syslog_mainbox \
    $$PWD/src/test_syslog_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += PROGRAMM_IN_UTF8
DEFINES += SYSLOG_LOG

DEFINES += UNDER_CONSTRUCTION

HEADERS += \
    test_syslog_mainbox.hpp \
    defines.hpp \
    version.hpp \

SOURCES += \
    test_syslog_mainbox.cpp \
    main.cpp

FORMS   += test_syslog_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

RESOURCES += \
    images/images.qrc \
    doc/doc.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

LIB_PATH = "../../../Qt/lib"

include ($$LIB_PATH/meta/mainwindow.pri)

include (src/sender_syslog/sender_syslog.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
