#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_ADB

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
    RC_ICONS = ico/computer.ico # нужно, чтобы создался файл .rc автоматом
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    images/images.qrc \
    scrcpy-server/scrcpy-server.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/opencv/opencv.pri)
include ($$LIB_PATH2/icons/arrows.pri)

include ($$LIB_PATH2/ethernet/tcp_local_server/tcp_local_server.pri)

include (src/test_adb_mainbox/test_adb_mainbox.pri)
include (src/autoclicker/autoclicker.pri)
include (src/adb_widget/adb_widget.pri)
include (src/ocv_widget/ocv_widget.pri)

VPATH = $$INCLUDEPATH
