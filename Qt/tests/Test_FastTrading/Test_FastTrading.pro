#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_FastTrading

FOLDER  = tests

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/template_fast_trading \
    $$PWD/src/template_fast_trading/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += SAVE_WIDGETS_CHECKBOX
DEFINES += SAVE_WIDGETS_SPINBOX
DEFINES += SAVE_WIDGETS_DOUBLESPINBOX

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    template_fast_trading.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    template_fast_trading.cpp \
    main.cpp

FORMS   += template_fast_trading.ui

win32 {
    RC_ICONS += ico/computer.ico
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    images/images.qrc \
    doc/doc.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

LIB_PATH  = "../../../Qt/lib"
LIB_PATH2 = "../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/meta/grapherbox.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
