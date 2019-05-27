# -------------------------------------------------
# Project created by QtCreator 2010-01-23T01:07:20
# -------------------------------------------------
TARGET = famique_qt_sdl
TEMPLATE = app
SOURCES += main.cpp \
    gui_mainwindow.cpp \
    sdlwidget.cpp \
    debugdialog.cpp \
    emulator.cpp \
    debugoutput.cpp \
    breakpoint_dialog.cpp \
    ../src_current/tracer.cpp \
    ../src_current/ppu.cpp \
    ../src_current/palette.cpp \
    ../src_current/famicom.cpp \
    ../src_current/definitions.cpp \
    ../src_current/cpu2.cpp \
    ../src_current/ppu2.cpp \
    ppu_viewer_dialog.cpp
HEADERS += gui_mainwindow.h \
    sdlwidget.h \
    debugdialog.h \
    ../src_current/tracer.h \
    ../src_current/ppu.h \
    ../src_current/palette.h \
    ../src_current/definitions.h \
    ../src_current/definition.h \
    ../src_current/cpu2.h \
    emulator.h \
    ../src_current/famicom.h \
    debugoutput.h \
    breakpoint_dialog.h \
    ../src_current/ppu2.h \
    ppu_viewer_dialog.h
INCLUDEPATH +=  C:\temp\SDL-1.2.14-ming\include\SDL # /Library/Frameworks/SDL.framework/Headers/
LIBS +=  -LC:\temp\SDL-1.2.14-ming\lib\ -lSDL

FORMS += gui_mainwindow.ui \
    debugdialog.ui \
    debugoutput.ui \
    breakpoint_dialog.ui \
    ppu_viewer_dialog.ui
