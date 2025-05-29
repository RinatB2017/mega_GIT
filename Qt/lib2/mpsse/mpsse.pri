#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

DEFINES += FT_VER_MAJOR=1
DEFINES += FT_VER_MINOR=0
DEFINES += FT_VER_BUILD=1

HEADERS += \
    ftdi_common.h \
    ftdi_infra.h \
    ftdi_mid.h \
    libmpsse_i2c.h \
    libmpsse_spi.h \
    ftd2xx.h \
    WinTypes.h

SOURCES += \
    ftdi_i2c.c \
    ftdi_infra.c \
    ftdi_mid.c \
    ftdi_spi.c \
    memcpy.c
