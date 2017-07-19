
TEMPLATE    = app
TARGET      = Test_event_filter

DEPENDPATH  += \
    $$PWD \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    canvaspicker.h \
    colorbar.h \
    plot.h \
    scalepicker.h

SOURCES += \
    canvaspicker.cpp \
    colorbar.cpp \
    event_filter.cpp \
    plot.cpp \
    scalepicker.cpp

LIB_PATH = "../lib"

include ($$LIB_PATH/qwt/qwt.pri)
include ($$LIB_PATH/turbo.pri)

VPATH = $$INCLUDEPATH
