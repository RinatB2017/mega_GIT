#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_HeroWars

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    targets/targets.qrc

OTHER_FILES += doc/notebook.txt

unix {
    CONFIG  += c++11

    LIBS    += -lX11 -lXtst
    LIBS    += -lOpenCL
}
win32 {
    LIBS += -luser32
    LIBS += -lgdi32
}

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

LIB_PATH  = "$$PWD/../../lib"
LIB_PATH2 = "$$PWD/../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/widgets/showpicture/showpicture.pri)
include ($$LIB_PATH2/opencv/opencv.pri)

include (src/myfindform/myfindform.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = $$PWD/src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

#message($$LIBS)

VPATH = $$INCLUDEPATH
