#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Template_old

FOLDER  = _Templates

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
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    ico/icons.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

LIB_PATH = "$$PWD/../../../lib"
include ($$LIB_PATH/meta/mainwindow.pri)

include (src/template_old_mainbox/template_old_mainbox.pri)

#message($$DEFINES)

VPATH = $$INCLUDEPATH
