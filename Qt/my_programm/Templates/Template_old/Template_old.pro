#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Template_old

FOLDER  = _Templates

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

win32 {
    VERSION = 0.99.20.1

    QMAKE_TARGET_COMPANY = Home
    QMAKE_TARGET_PRODUCT = $$TARGET
    QMAKE_TARGET_COPYRIGHT = "Copyright (c) 2020-2025"
    QMAKE_TARGET_DESCRIPTION = "my description"

    RC_ICONS = "ico/computer.ico"
    # RC_FILE  = programm.rc
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    ico/icons.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

LIB_PATH = "$$PWD/../../../lib"
include (src/config.pri)    #строка должна быть перед mainwindow
include ($$LIB_PATH/meta/mainwindow.pri)

include (src/template_old_mainbox/template_old_mainbox.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = $$PWD/src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

message($$DEFINES)

VPATH = $$INCLUDEPATH
