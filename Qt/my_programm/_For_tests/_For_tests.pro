#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = _For_tests

include (conf/conf.pri)

QT  += concurrent

PROGRAMM_PATH  += \
    $$PWD/src
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

HEADERS += \
    structs.h \
    defines.hpp
SOURCES += \
    main.cpp

RESOURCES += \
    $$PWD/themes/themes_css/themes_css.qrc \
    $$PWD/themes/themes_qss/themes_qss.qrc \
    $$PWD/images/bullets/bullets.qrc \
    $$PWD/styles/styles.qrc \
    $$PWD/images/masks/masks.qrc \
    $$PWD/images/other/other.qrc \
    $$PWD/ico/icons.qrc

#можно поставить новое logo
#RESOURCES += about/about.qrc

OTHER_FILES += \
    doc/notebook.txt

# профилирование
#include (conf/prof.pri)

LIB_PATH  = "$$PWD/../../lib"
LIB_PATH2 = "$$PWD/../../lib2"

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

win32 {
    RC_ICONS = ico/computer.ico
}

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH/memories/memories.pri)

include ($$LIB_PATH2/connection/connection.pri)
include ($$LIB_PATH2/icons/digits.pri)

include ($$LIB_PATH/mywidgets/mylistwidget/mylistwidget.pri)
include ($$LIB_PATH2/widgets/QLongLongSpinBox/QLongLongSpinBox.pri)

include ($$LIB_PATH2/widgets/show_hsv_color/show_hsv_color.pri)
include ($$LIB_PATH2/widgets/color_widget/color_widget.pri)

include (src/for_tests_mainbox_gui/for_tests_mainbox_gui.pri)
include (src/for_tests_mainbox/for_tests_mainbox.pri)

include (src/mymainwindow/mymainwindow.pri)
include (src/widgets/widgets.pri)

# include (src/cube/cube.pri)
# include (src/ogl/ogl.pri)

#message ($$QMAKESPEC)

VPATH = $$INCLUDEPATH
