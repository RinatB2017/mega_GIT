#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

SOURCES += simple_tcp_server.cpp
HEADERS += simple_tcp_server.hpp

win32 {
    # Отключает предупреждения MSVC о небезопасных функциях (fopen, strcpy и т.д.)
    DEFINES += _CRT_SECURE_NO_WARNINGS
}
