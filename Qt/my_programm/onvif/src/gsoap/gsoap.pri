
INCLUDEPATH += \
    $$PWD \
    $$PWD/inc \
    $$PWD/src

# Input
HEADERS += inc/duration.h \
           inc/mecevp.h \
           inc/smdevp.h \
           inc/stdsoap2.h \
           inc/threads.h \
           inc/wsaapi.h \
           inc/wsseapi.h \
           inc/stdsoap2.h \
           inc/mecevp.h \
           inc/smdevp.h \
           inc/threads.h \
           inc/wsaapi.h \
           inc/wsseapi.h

SOURCES += src/dom.cpp \
           src/duration.c \
           src/mecevp.c \
           src/smdevp.c \
           src/stdsoap2.cpp \
           src/threads.c \
           src/wsaapi.c \
           src/wsseapi.cpp
