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

QT  += webenginewidgets

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += USE_CUSTOMPAGE

#DEFINES += SAVE_INI

DEFINES += USE_TOPLEVELWIDGETS

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
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/QXmlPutGet/qxmlputget.pri)
include ($$LIB_PATH2/highlighter/highlighter.pri)
include ($$LIB_PATH2/codeeditor/codeeditor.pri)

include (src/multiedit/multiedit.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH

RESOURCES += \
    proxy/proxies.qrc
