#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_wwWidgets

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_wwWidgets_mainbox \
    $$PWD/src/test_wwWidgets_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#QT  += serialbus

HEADERS += \
    test_wwWidgets_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_wwWidgets_mainbox.cpp \
    main.cpp

FORMS   += test_wwWidgets_mainbox.ui
#FORMS   += test_wwWidgets_mainbox_test.ui

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

LIB_PATH = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

include ($$LIB_PATH2/wwWidgets/wwWidgets.pri)

#!exists(OBJECTS_DIR) {
#    VERSION_HEADER = $$PWD/src/version.hpp
#    include ($$LIB_PATH/auto_inc_version.pri)
#}

CONFIG(debug, debug|release) {
    message(debug mode)
}
else {
    message(release mode)
}

VPATH = $$INCLUDEPATH
