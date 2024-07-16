#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_HC-05

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_hc05_mainbox \
    $$PWD/src/test_hc05_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QT  += serialport

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

win32 {
    RC_FILE = ico/bluetooth.ico
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    images/images.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

#----------------------------------------------
include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)

include ($$LIB_PATH/meta/mainwindow.pri)

include (src/test_hc05_mainbox/test_hc05_mainbox.pri)
#----------------------------------------------

VPATH = $$INCLUDEPATH
