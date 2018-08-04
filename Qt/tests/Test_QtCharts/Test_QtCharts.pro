#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_QtCharts

FOLDER  = tests

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/Test_QtCharts_mainbox \
    $$PWD/src/Test_QtCharts_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QT  += charts

#QMAKE_CXXFLAGS += -fno-show-column

#DEFINES += LOGO_GL
#DEFINES += FIXED_SIZE

#
#DEFINES += NO_STYLETOOLBAR
#DEFINES += NO_STATUSBAR
DEFINES += NO_TRAYICON
#DEFINES += NO_TOOLBAR
#DEFINES += NO_MENU
#DEFINES += NO_LOG

#DEFINES += ONLY_ENGLISH

#DEFINES += LOG_READ_ONLY

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
    Test_QtCharts_mainbox.hpp

SOURCES += \
    Test_QtCharts_mainbox.cpp \
    main.cpp

FORMS   += Test_QtCharts_mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

RESOURCES += images/images.qrc

OTHER_FILES += doc/notebook.txt

LIB_PATH = "../../../Qt/lib"
include ($$LIB_PATH/meta/mainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
