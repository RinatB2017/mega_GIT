
# Variables
VERSION = '1.0'
TEMPLATE = app

# -Wextra -Weffc++
QMAKE_CXXFLAGS = -Wall
QMAKE_CXXFLAGS_RELEASE = -Wall

QT += core gui widgets

CONFIG += release warn_on qt debug

OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build

TARGET = fairseabattle

DEFINES += FAIRSEABATTLEVERSION=$$VERSION
DEFINES += FAIRSEABATTLEPRERELEASE=pre2

DEPENDPATH += src/logic src/control src/gui
INCLUDEPATH += src/logic src/control src/gui

DISTFILES += COPYING MANIFEST README
DISTFILES += images/new.png \
             images/auto.png \
             images/go.png

RESOURCES= fairseabattle.qrc

HEADERS += src/gui/arenawidget.h \
           src/gui/alerter.h \
           src/control/controller.h \
           src/gui/flags_colors.h \
           src/logic/geometry.h \
           src/logic/map.h \
           src/logic/map_io.h \
           src/logic/arena.h \
           src/logic/flags.h \
           src/logic/legend.h \
           src/logic/legend_io.h \
           src/logic/flags_io.h \
           src/logic/arena_io.h

SOURCES += src/fairseabattle.cpp \
           src/gui/alerter.cpp \
           src/gui/arenawidget.cpp \
           src/control/controller.cpp \
           src/gui/flags_colors.cpp \
           src/logic/arena.cpp \
           src/logic/geometry.cpp \
           src/logic/map.cpp \
           src/logic/map_io.cpp \
           src/logic/legend_io.cpp \
           src/logic/flags.cpp \
           src/logic/legend.cpp \
           src/logic/flags_io.cpp \
           src/logic/arena_io.cpp

unix {
  install_fairseabattle.path = /usr/local/bin
  install_fairseabattle.commands = $(INSTALL_PROGRAM) $(TARGET) $(INSTALL_ROOT)$$install_fairseabattle.path/$(QMAKE_TARGET)
  INSTALLS += install_fairseabattle
} else {
  message("No install target for your platform.")
  message("Install FairSeaBattle manualy.")
}
