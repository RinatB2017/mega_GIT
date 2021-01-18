#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = _For_tests

PROGRAMM_PATH  += \
    $$PWD/src
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

CONFIG(debug, debug|release) {
    # профилирование
#    QMAKE_CXXFLAGS_DEBUG += -pg
#    QMAKE_LFLAGS_DEBUG += -pg
    #---
}

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico # нужно, чтобы создался фал .rc автоматом
#    FILEVERSION = VER_MAJOR,VER_MINOR,VER_BUILD,0
#    RC_FILE  = programm.rc
}

RESOURCES += \
    images/puzzle/puzzle.qrc \
    images/images.qrc \
    images/bullets/bullets.qrc \
    ico/icons.qrc \
    themes/themes_css/themes_css.qrc \
    themes/themes_qss/themes_qss.qrc 

#можно поставить новое logo
#RESOURCES += about/about.qrc

OTHER_FILES += \
    doc/notebook.txt

LIB_PATH  = "$$PWD/../../lib"
LIB_PATH2 = "$$PWD/../../lib2"

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

include (src/config.pri)
include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/icons/digits.pri)
include ($$LIB_PATH2/icons/arrows.pri)

include ($$LIB_PATH/mywidgets/mylistwidget/mylistwidget.pri)
include ($$LIB_PATH2/widgets/QLongLongSpinBox/QLongLongSpinBox.pri)
#include ($$LIB_PATH2/web/mybrowser/mybrowser.pri)

include (src/for_tests_mainbox/for_tests_mainbox.pri)
include (src/mymainwindow/mymainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = $$PWD/src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
