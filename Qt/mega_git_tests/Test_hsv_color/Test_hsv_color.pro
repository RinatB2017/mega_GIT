#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_hsv_color

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

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
    images/images.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include (src/config.pri)    #строка должна быть перед mainwindow
include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/widgets/show_hsv_color/show_hsv_color.pri)
include ($$LIB_PATH2/widgets/color_widget/color_widget.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = $$PWD/src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
