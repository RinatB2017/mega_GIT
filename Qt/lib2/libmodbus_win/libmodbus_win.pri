
DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH += $$DEPENDPATH

# Input
HEADERS += \
    modbus-private.h \
    modbus-rtu-private.h \
    modbus-rtu.h \
#    modbus-tcp-private.h \
#    modbus-tcp.h \
    modbus-version.h \
    modbus.h \
    config.h
    
SOURCES += \
    modbus-data.c \
    modbus-rtu.c \
#    modbus-tcp.c \
    modbus.c
