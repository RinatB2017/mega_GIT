#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Reader_SAAT_T505

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/reader_saat_t505_mainbox \
    $$PWD/src/reader_saat_t505_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

#DEFINES += RS232_FIXED_SIZE
#DEFINES += RS232_SEND
#DEFINES += RS232_LOG

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += NO_RESIZE

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp \
    reader_saat_t505_mainbox.hpp

SOURCES += \
    reader_saat_t505_mainbox.cpp \
    main.cpp

FORMS   += reader_saat_t505_mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    images/images.qrc \
    doc/doc.qrc

OTHER_FILES += doc/notebook.txt

#----------------------------------------------
LIB_PATH = "../../lib"
LIB_PATH2 = "../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

greaterThan(QT_MAJOR_VERSION, 4) {
    include ($$LIB_PATH2/serial5/serial5.pri)
} esle {
    include ($$LIB_PATH2/serial4/serial4.pri)
}

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}
#----------------------------------------------

VPATH = $$INCLUDEPATH
