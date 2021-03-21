#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_QTermWidget

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    /usr/include/qtermwidget5 \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

LIBS    += -lqtermwidget5

win32 {
    RC_ICONS = ico/computer.ico
    error (Only linux)
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    images/images.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

include (src/test_qtermwidget_mainbox/test_qtermwidget_mainbox.pri)

VPATH = $$INCLUDEPATH
