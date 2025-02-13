
DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH += $$DEPENDPATH

# Input
HEADERS += \
    src/modbus-ascii-private.h \
    src/modbus-ascii.h \
    src/modbus-private.h \
    src/modbus-rtu-private.h \
    src/modbus-rtu.h \
    src/modbus-tcp-private.h \
    src/modbus-tcp.h \
    src/modbus-version.h \
    src/modbus.h \
    src/config.h
    
SOURCES += \
    src/modbus-ascii.c \
    src/modbus-data.c \
    src/modbus-rtu.c \
    src/modbus-tcp.c \
    src/modbus.c
