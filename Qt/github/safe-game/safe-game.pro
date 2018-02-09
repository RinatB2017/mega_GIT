QT += core gui widgets

TARGET = safe-game
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

PROGRAMM_PATH  += \
    $$PWD \
    $$PWD/src
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

SOURCES += \
    main.cpp \
    main_window.cpp \
    safe_widget.cpp \
    switch_widget.cpp \
    hall_of_fame_dialog.cpp \
    utils.cpp \
    switch_command.cpp \
    history.cpp

HEADERS += \
    main_window.h \
    safe_widget.h \
    switch_widget.h \
    orientation.h \
    hall_of_fame_dialog.h \
    utils.h \
    switch_command.h \
    history.h

RESOURCES += \
    resources/resources.qrc

VPATH = $$INCLUDEPATH
