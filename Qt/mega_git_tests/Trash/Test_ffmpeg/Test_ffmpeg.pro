#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_ffmpeg

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    images/images.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

LIB_PATH  = "$$PWD/../../lib"
LIB_PATH2 = "$$PWD/../../lib2"

LIBS += -lavformat \
        -lavcodec \
        -lavdevice \
        -lavfilter \
        -lavutil \
        -lavutil \
        -lavresample \
        -lswscale \

include ($$LIB_PATH/meta/mainwindow.pri)

include (src/test_ffmpeg_mainbox_GUI/test_ffmpeg_mainbox_GUI.pri)
include (src/test_ffmpeg_mainbox/test_ffmpeg_mainbox.pri)

VPATH = $$INCLUDEPATH
