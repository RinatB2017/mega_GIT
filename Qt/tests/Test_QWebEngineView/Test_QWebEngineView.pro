#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_QWebEngineView

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_qwebengineview_mainbox \
    $$PWD/src/test_qwebengineview_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#QMAKE_CXXFLAGS += -fno-show-column
QT  += webenginewidgets

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += SAVE_INI

DEFINES += SAVE_WIDGETS_TEXTEDIT
DEFINES += SAVE_WIDGETS_SPLITTER
DEFINES += SAVE_WIDGETS_CHECKBOX

HEADERS += \
    test_qwebengineview_mainbox.hpp \
    custompage.h \
    defines.hpp \
    version.hpp

SOURCES += \
    test_qwebengineview_mainbox.cpp \
    main.cpp

FORMS   += test_qwebengineview_mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH  = "../../../Qt/lib"
LIB_PATH2 = "../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/QXmlPutGet/qxmlputget.pri)
include ($$LIB_PATH2/highlighter/highlighter.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH

RESOURCES += \
    proxy/proxies.qrc
