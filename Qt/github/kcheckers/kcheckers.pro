TEMPLATE = app

CONFIG  += qt warn_on

QT     += widgets

PROGRAMM_PATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

HEADERS	= \
    pdn.h \
    checkers.h \
    echeckers.h \
    rcheckers.h \
    field.h \
    toplevel.h \
    view.h \
    history.h \
    board.h \
    newgamedlg.h \
    common.h \
    player.h \
    humanplayer.h \
    computerplayer.h

SOURCES	= \
    pdn.cc \
    checkers.cc \
    echeckers.cc \
    rcheckers.cc \
    field.cc \
    toplevel.cc \
    view.cc \
    history.cc \
    board.cc \
    main.cc \
    newgamedlg.cc \
    humanplayer.cc \
    computerplayer.cc

RESOURCES = \
    kcheckers.qrc


TARGET		= kcheckers
PREFIX		= $$system(grep 'define PREFIX' common.h | cut -d'"' -f2)
SHARE_PATH	= $$system(grep 'define SHARE_PATH' common.h | cut -d'"' -f2)

TRANSLATIONS	+= i18n/kcheckers_ru.ts
#		i18n/kcheckers_ru.ts

#target.path	= $$PREFIX/bin
#INSTALLS	+= target


#
# This hack is needed for i18n support.
#
share.path	+= $$PREFIX/share/kcheckers
share.files	+= kcheckers.pdn COPYING AUTHORS ChangeLog README themes i18n/*
INSTALLS	+= share

VPATH = $$INCLUDEPATH
