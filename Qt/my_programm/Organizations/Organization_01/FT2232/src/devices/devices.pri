#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD \
    $$PWD/ad8400 \
    $$PWD/bitbang \
    $$PWD/hd44780 \
    $$PWD/at93c56 \
    $$PWD/mcp4922 \
    $$PWD/spi \
    $$PWD/indicator \
    $$PWD/ds18b20

INCLUDEPATH = $$DEPENDPATH

HEADERS += ad8400.hpp
SOURCES += ad8400.cpp

HEADERS += bitbang.hpp
SOURCES += bitbang.cpp

HEADERS += hd44780.hpp
SOURCES += hd44780.cpp

HEADERS += at93c56.hpp
SOURCES += at93c56.cpp

HEADERS += mcp4922.hpp
SOURCES += mcp4922.cpp

HEADERS += spi.hpp
SOURCES += spi.cpp

HEADERS += indicator.hpp
SOURCES += indicator.cpp
HEADERS += indicator_thread.hpp
SOURCES += indicator_thread.cpp

HEADERS += ds18b20.hpp
SOURCES += ds18b20.cpp

include (ftdi/ftdi.pri)
