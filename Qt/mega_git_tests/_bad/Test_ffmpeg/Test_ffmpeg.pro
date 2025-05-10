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

win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}

LIBS += \
    -lavcodec \
    -lswresample \
    -lavutil \
    -lavformat \
    -lswscale \
    -lm

include ($$LIB_PATH/meta/mainwindow.pri)

include (src/test_ffmpeg_mainbox_GUI/test_ffmpeg_mainbox_GUI.pri)
include (src/test_ffmpeg_mainbox/test_ffmpeg_mainbox.pri)
include (src/tester/tester.pri)
include (src/creator/creator.pri)

VPATH = $$INCLUDEPATH
