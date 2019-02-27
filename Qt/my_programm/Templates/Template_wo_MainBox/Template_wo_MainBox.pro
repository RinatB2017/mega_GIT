#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Template_wo_MainBox

FOLDER  = _Templates

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/newmainwindow
INCLUDEPATH = $$DEPENDPATH

#QT  += serialbus

#DEFINES += LOGO_GL
#DEFINES += FIXED_SIZE
#
#DEFINES += NO_STYLETOOLBAR
#DEFINES += NO_STATUSBAR
DEFINES += NO_TRAYICON
#DEFINES += NO_TOOLBAR
DEFINES += NO_RESIZE
#DEFINES += NO_MENU
#DEFINES += NO_LOG

#DEFINES += ONLY_ENGLISH

#DEFINES += LOG_READ_ONLY

#DEFINES += SAVE_WIDGETS_CHECKBOX
#DEFINES += SAVE_WIDGETS_COMBOBOX
#DEFINES += SAVE_WIDGETS_PUSHBUTTON
#DEFINES += SAVE_WIDGETS_TOOLBUTTON
#DEFINES += SAVE_WIDGETS_SPINBOX
#DEFINES += SAVE_WIDGETS_DOUBLESPINBOX
#DEFINES += SAVE_WIDGETS_SLIDER
#DEFINES += SAVE_WIDGETS_TEXTEDIT
#DEFINES += SAVE_WIDGETS_LINEEDIT
#DEFINES += SAVE_WIDGETS_SPLITTER

#DEFINES += SYSLOG_LOG

#DEFINES += DEMO
#DEFINES += "DEMO_YEAR=2017"
#DEFINES += "DEMO_MONTH=9"
#DEFINES += "DEMO_DAY=9"

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    newmainwindow.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    newmainwindow.cpp \
    main.cpp

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
LIB_PATH = "../../../lib"
LIB_PATH2 = "../../../lib2"
include ($$LIB_PATH/meta/mainwindow.pri)

###
include ($$LIB_PATH2/serial5/serial5.pri)
include ($$LIB_PATH2/GrapherBox/GrapherBox.pri)
include ($$LIB_PATH2/csvreader/csvreader.pri)
include ($$LIB_PATH2/qwt/qwt.pri)
###

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}
#----------------------------------------------

VPATH = $$INCLUDEPATH
