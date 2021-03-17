#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = _For_tests

include (conf/conf.pri)

QT += uitools

PROGRAMM_PATH  += \
    $$PWD/src
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

HEADERS += \
    defines.hpp
    version.hpp

SOURCES += \
    main.cpp

RESOURCES += \
    $$PWD/styles/styles.qrc \
    $$PWD/images/bullets/bullets.qrc \
    $$PWD/ico/icons.qrc \
    $$PWD/themes/themes_css/themes_css.qrc \
    $$PWD/themes/themes_qss/themes_qss.qrc

#можно поставить новое logo
#RESOURCES += about/about.qrc

OTHER_FILES += \
    doc/notebook.txt

LIB_PATH  = "$$PWD/../../lib"
LIB_PATH2 = "$$PWD/../../lib2"

CONFIG(debug, debug|release) {
    include (src/test/test.pri)

    # профилирование
    # QMAKE_CXXFLAGS_DEBUG += -pg
    # QMAKE_LFLAGS_DEBUG += -pg
    #---
}

win32 {
    RC_ICONS = ico/computer.ico
}

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/icons/digits.pri)

include ($$LIB_PATH/mywidgets/mylistwidget/mylistwidget.pri)
include ($$LIB_PATH2/widgets/QLongLongSpinBox/QLongLongSpinBox.pri)

include (src/for_tests_mainbox/for_tests_mainbox.pri)
include (src/mymainwindow/mymainwindow.pri)

include (src/widgets/widgets.pri)

#!exists(OBJECTS_DIR) {
#    VERSION_HEADER = $$PWD/src/version.hpp
#    message($$VERSION_HEADER)
#    include ($$LIB_PATH/auto_inc_version.pri)
#}

VPATH = $$INCLUDEPATH
