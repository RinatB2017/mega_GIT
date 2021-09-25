#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_QML

FOLDER  = tests

QT  += qml quick

include (conf/conf.pri)

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
    images/images.qrc \
    qml/qml.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

include (src/test_qml_mainbox_GUI/test_qml_mainbox_GUI.pri)
include (src/test_qml_mainbox/test_qml_mainbox.pri)

VPATH = $$INCLUDEPATH
