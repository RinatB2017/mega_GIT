#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_SerialTerm

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    /usr/include/qtermwidget6 \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

LIBS    += -lqtermwidget6

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

include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serialwidget/serialwidget.pri)
include ($$LIB_PATH2/serial5/serialbox5_fix_baudrate/serialbox5_fix_baudrate.pri)

include (src/test_serialterm_mainbox/test_serialterm_mainbox.pri)
include (src/serialterm/serialterm.pri)

VPATH = $$INCLUDEPATH
