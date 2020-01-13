#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_QDBus

FOLDER  = tests

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/test_qdbus_mainbox \
    $$PWD/src/test_qdbus_mainbox/ui
INCLUDEPATH = $$DEPENDPATH



QT  += dbus

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += SHOW_SIZE

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    test_qdbus_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_qdbus_mainbox.cpp \
    main.cpp

FORMS   += test_qdbus_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
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
LIB_PATH = "$$PWD/../../../Qt/lib"

include ($$LIB_PATH/meta/mainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}
#----------------------------------------------

VPATH = $$INCLUDEPATH
