#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_Mask

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
    RC_ICONS = "ico/computer.ico"
    # RC_FILE  = programm.rc
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    ico/icons.qrc \
    images/resources.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

include (src/test_mask_mainbox/test_mask_mainbox.pri)

#message($$DEFINES)

VPATH = $$INCLUDEPATH
