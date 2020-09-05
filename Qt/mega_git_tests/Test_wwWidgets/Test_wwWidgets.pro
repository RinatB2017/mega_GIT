#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_wwWidgets

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_wwWidgets_mainbox \
    $$PWD/src/test_wwWidgets_mainbox/ui
INCLUDEPATH = $$DEPENDPATH



#QT  += serialbus

#DEFINES += LOGO_GL
#DEFINES += FIXED_SIZE

#DEFINES += NO_TESTBAR_GRAPHER
#DEFINES += NO_STYLETOOLBAR
#DEFINES += NO_STATUSBAR
DEFINES += NO_TRAYICON
#DEFINES += NO_TOOLBAR
#DEFINES += NO_MENU
#DEFINES += NO_LOG

#DEFINES += ONLY_ENGLISH

#DEFINES += LOG_READ_ONLY

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    test_wwWidgets_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_wwWidgets_mainbox.cpp \
    main.cpp

FORMS   += test_wwWidgets_mainbox.ui
#FORMS   += test_wwWidgets_mainbox_test.ui

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

LIB_PATH = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

include ($$LIB_PATH2/wwWidgets/wwWidgets.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

CONFIG(debug, debug|release) {
    message(debug mode)
}
else {
    message(release mode)
}

VPATH = $$INCLUDEPATH
