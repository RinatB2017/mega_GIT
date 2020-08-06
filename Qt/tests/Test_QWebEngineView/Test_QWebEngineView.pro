#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_QWebEngineView

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QT  += webenginewidgets

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += USE_CUSTOMPAGE
#DEFINES += SAVE_INI

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/highlighter/highlighter.pri)
include ($$LIB_PATH2/codeeditor/codeeditor.pri)
include ($$LIB_PATH2/web/mybrowser/mybrowser.pri)
include ($$LIB_PATH2/widgets/multiedit/multiedit.pri)

include (src/test_qwebengineview_mainbox/test_qwebengineview_mainbox.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

RESOURCES += \
    proxy/proxies.qrc

VPATH = $$INCLUDEPATH
