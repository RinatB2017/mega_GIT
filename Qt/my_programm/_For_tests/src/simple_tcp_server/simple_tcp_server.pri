#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

win32 {
    # Отключает предупреждения MSVC о небезопасных функциях (fopen, strcpy и т.д.)
    DEFINES += _CRT_SECURE_NO_WARNINGS
}

HEADERS += simpletcpserver.h
SOURCES += simpletcpserver.cpp
