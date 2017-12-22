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
    ppu_viewer_dialog.cpp \
    ../emulator/ppu/ppu2.cpp \
    ../emulator/ppu/ppu.cpp \
    ../emulator/ppu/palette.cpp \
    ../emulator/cpu/tracer.cpp \
    ../emulator/cpu/definitions.cpp \
    ../emulator/cpu/cpu2.cpp \
    ../emulator/apu/apu.cpp \
    ../emulator/memory.cpp \
    ../emulator/famicom.cpp \
    ../emulator/mappers/mapper.cpp \
    ../emulator/apu/nes_apu/Nes_Apu.cpp \
    ../emulator/apu/nes_apu/Blip_Buffer.cpp \
    ../emulator/apu/nes_apu/Nonlinear_Buffer.cpp \
    ../emulator/apu/nes_apu/Nes_Vrc6.cpp \
    ../emulator/apu/nes_apu/Nes_Oscs.cpp \
    ../emulator/apu/nes_apu/Nes_Namco.cpp \
    ../emulator/apu/nes_apu/Multi_Buffer.cpp \
    ../emulator/apu/nes_apu/apu_snapshot.cpp \
    ../emulator/apu/Sound_Queue.cpp \
    ../emulator/nescart.cpp \
    ../emulator/mappers/nrom.cpp \
    ../emulator/mappers/mmc1.cpp \
    ../emulator/mappers/cnrom.cpp \
    ../emulator/mappers/uxrom.cpp \
    ../emulator/input.cpp \
    ../emulator/config_file_parser.cpp \
    settings.cpp \ # /Library/Frameworks/SDL.framework/SDLmain.m
    sdlframelimiter.cpp \
    memoryviewer.cpp \
    ../emulator/ppu/ppu3.cpp \
    ../emulator/ppu/ppu_base.cpp

HEADERS += gui_mainwindow.h \
    sdlwidget.h \
    debugdialog.h \
    emulator.h \
    debugoutput.h \
    breakpoint_dialog.h \
    ppu_viewer_dialog.h \
    ../emulator/apu/apu.h \
    ../emulator/cpu/tracer.h \
    ../emulator/cpu/definitions.h \
    ../emulator/cpu/cpu2.h \
    ../emulator/ppu/ppu2.h \
    ../emulator/ppu/ppu.h \
    ../emulator/ppu/palette.h \
    ../emulator/memory.h \
    ../emulator/famicom.h \
    ../emulator/mappers/mapper.h \
    ../emulator/apu/Sound_Queue.h \
    ../emulator/apu/boost/static_assert.hpp \
    ../emulator/apu/boost/cstdint.hpp \
    ../emulator/apu/boost/config.hpp \
    ../emulator/apu/nes_apu/Nes_Vrc6.h \
    ../emulator/apu/nes_apu/Nes_Oscs.h \
    ../emulator/apu/nes_apu/Nes_Namco.h \
    ../emulator/apu/nes_apu/Nes_Apu.h \
    ../emulator/apu/nes_apu/Multi_Buffer.h \
    ../emulator/apu/nes_apu/Blip_Synth.h \
    ../emulator/apu/nes_apu/Blip_Buffer.h \
    ../emulator/apu/nes_apu/blargg_source.h \
    ../emulator/apu/nes_apu/blargg_common.h \
    ../emulator/apu/nes_apu/apu_snapshot.h \
    ../emulator/apu/nes_apu/Nonlinear_Buffer.h \
    ../emulator/nescart.h \
    ../emulator/mappers/nrom.h \
    ../emulator/mappers/mmc1.h \
    ../emulator/mappers/cnrom.h \
    ../emulator/mappers/uxrom.h \
    ../emulator/input.h \
    ../emulator/config_file_parser.h \
    settings.h \
    sdlframelimiter.h \
    memoryviewer.h \
    ../emulator/ppu/ppu3.h \
    ../emulator/ppu/ppu_base.h
INCLUDEPATH +=  \
    /usr/include/SDL \
    ../emulator \
    ../emulator/apu/nes_apu \
    ../emulator/apu # C:\temp\nvwa-0.8.2\nvwa \

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    message(Qt5 = $$QT)
}
else {
    message(Qt4 = $$QT)
}

# -L/Library/Frameworks/SDL.framework/ -lSDL
LIBS += -lSDL
LIBS    += -lSDL_ttf
LIBS    += -lSDL_mixer
LIBS    += -lSDL_image


# SDL ## -framework SDL -lSDL
FORMS += gui_mainwindow.ui \
    debugdialog.ui \
    debugoutput.ui \
    breakpoint_dialog.ui \
    ppu_viewer_dialog.ui \
    settings.ui \
    memoryviewer.ui
OTHER_FILES += ../readme.txt \
    ../../6502.txt
