#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_MODBUS

FOLDER  = modbus

include (conf/conf.pri)

DEPENDPATH  += \
    /usr/include/modbus \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QT      += serialport

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp \

OTHER_FILES += doc/notebook.txt

RESOURCES += \
    src/master.qrc

win32 {
    RC_ICONS = ico/computer.ico
}

QT  += serialbus

include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)

include ($$LIB_PATH/meta/mainwindow.pri)

include (src/test_modbus_mainbox/test_modbus_mainbox.pri)

VPATH = $$INCLUDEPATH
