#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_QDBus

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_qdbus_mainbox \
    $$PWD/src/test_qdbus_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QT  += dbus

HEADERS += \
    test_qdbus_mainbox.hpp \
    defines.hpp

SOURCES += \
    test_qdbus_mainbox.cpp \
    main.cpp

FORMS   += test_qdbus_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    images/images.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

#----------------------------------------------
include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)

include ($$LIB_PATH/meta/mainwindow.pri)
#----------------------------------------------

VPATH = $$INCLUDEPATH
