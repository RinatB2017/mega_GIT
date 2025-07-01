#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_CuteReport

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
    data/data.qrc \
    music/music.qrc \
    images/images.qrc

OTHER_FILES += \
    doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

#-----------------------------------------------------------
win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}
#-----------------------------------------------------------
include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/cutereport/cutereport.pri)

include ($$PWD/src/test_CuteReport_mainbox/test_CuteReport_mainbox.pri)
#-----------------------------------------------------------
VPATH = $$INCLUDEPATH
