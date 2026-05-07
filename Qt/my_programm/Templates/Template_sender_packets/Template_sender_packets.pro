#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Template_sender_packets

FOLDER  = _Templates

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    packets.h \
    defines.hpp

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/RS232.ico
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    music/music.qrc \
    ico/icons.qrc

OTHER_FILES += \
    doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}
#----------------------------------------------
win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}
#----------------------------------------------
include ($$LIB_PATH/meta/mainwindow.pri)

include ($$LIB_PATH2/serial5/serialwidget/serialwidget.pri)
include ($$LIB_PATH2/serial5/serialbox5_fix_baudrate/serialbox5_fix_baudrate.pri)

include ($$LIB_PATH2/crc/crc.pri)

include ($$LIB_PATH2/power_knock/power_knock.pri)
include ($$LIB_PATH2/icons/digits.pri)
include ($$LIB_PATH2/icons/arrows.pri)

include (src/template_sender_packets_mainbox/template_sender_packets_mainbox.pri)
#----------------------------------------------

VPATH = $$INCLUDEPATH
