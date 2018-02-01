
TEMPLATE    = app
TARGET      = Test_event_filter

FOLDER  = tests

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

LIB_PATH = "../../lib"
LIB_PATH2 = "../../lib2"

include ($$LIB_PATH/turbo.pri)
include ($$LIB_PATH2/qwt/qwt.pri)

VPATH = $$INCLUDEPATH
