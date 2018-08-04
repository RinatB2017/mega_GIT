#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_QStateMachine

FOLDER  = tests

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/Test_QStateMachine_mainbox \
    $$PWD/src/Test_QStateMachine_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp \
    Test_QStateMachine_mainbox.hpp

SOURCES += \
    Test_QStateMachine_mainbox.cpp \
    main.cpp

FORMS   += Test_QStateMachine_mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    images/images.qrc \
    doc/doc.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

LIB_PATH = "../../../Qt/lib"

include ($$LIB_PATH/meta/mainwindow.pri)

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
