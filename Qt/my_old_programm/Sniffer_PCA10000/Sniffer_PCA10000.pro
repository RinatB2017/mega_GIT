#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Sniffer_PCA10000

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/sniffer_pca10000_mainbox \
    $$PWD/src/sniffer_pca10000_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#QMAKE_CXXFLAGS += -fno-show-column

QT      += serialport

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += SAVE_WIDGETS_CHECKBOX
DEFINES += SAVE_WIDGETS_COMBOBOX
DEFINES += SAVE_WIDGETS_PUSHBUTTON
DEFINES += SAVE_WIDGETS_TOOLBUTTON
DEFINES += SAVE_WIDGETS_SPINBOX
DEFINES += SAVE_WIDGETS_DOUBLESPINBOX
DEFINES += SAVE_WIDGETS_SLIDER
DEFINES += SAVE_WIDGETS_TEXTEDIT
DEFINES += SAVE_WIDGETS_LINEEDIT

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp \
    sniffer_pca10000_mainbox.hpp

SOURCES += \
    sniffer_pca10000_mainbox.cpp \
    main.cpp

FORMS   += sniffer_pca10000_mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

include (src/test/test.pri)

include (src/PCA10000/pca10000.pri)

LIB_PATH = "../../lib"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH/time/time.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
