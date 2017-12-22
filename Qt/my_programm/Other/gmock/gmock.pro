
TEMPLATE    = app
TARGET      = gmock_test

DEPENDPATH  += \
    $$PWD\src
INCLUDEPATH = $$DEPENDPATH

SOURCES += main.cpp

LIBS    += -lgtest
LIBS    += -lgmock
