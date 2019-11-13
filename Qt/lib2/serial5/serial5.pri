

DEPENDPATH 	+= \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH	= $$DEPENDPATH

QT  *=  serialport

HEADERS += serialbox5.hpp
SOURCES += serialbox5.cpp
FORMS	+= \
    serialbox5.ui \
    sendbox5.ui

HEADERS += fake_serialbox5.hpp
SOURCES += fake_serialbox5.cpp
    
HEADERS += sendbox5.hpp
SOURCES += sendbox5.cpp

HEADERS += serialbox5_thread.hpp
SOURCES += serialbox5_thread.cpp

HEADERS += serialbox5_lite.hpp
SOURCES += serialbox5_lite.cpp
FORMS	+= serialbox5_lite.ui

HEADERS += serialbox5_fix_baudrate.hpp
SOURCES += serialbox5_fix_baudrate.cpp
FORMS	+= serialbox5_fix_baudrate.ui

HEADERS += serialbox5_fix_baudrate_win7.hpp
SOURCES += serialbox5_fix_baudrate_win7.cpp
FORMS   += serialbox5_fix_baudrate_win7.ui

include (../power_knock/power_knock.pri)
