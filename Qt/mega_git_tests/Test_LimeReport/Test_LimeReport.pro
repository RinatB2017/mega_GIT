#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_LimeReport

FOLDER  = tests

include (conf/conf.pri)

QT      += printsupport
QT      += qml

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
    music/music.qrc \
    images/images.qrc

OTHER_FILES += doc/notebook.txt

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
include ($$LIB_PATH2/limereport/limereport.pri)

include ($$PWD/src/test_LimeReport_mainbox/test_LimeReport_mainbox.pri)
#-----------------------------------------------------------
VPATH = $$INCLUDEPATH
