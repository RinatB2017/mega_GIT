#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = "PETCat"

FOLDER  = organizations/Organization_02

#DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

linux {
    DEPENDPATH  += $$(HOME)/BitTorrent_sync/WorkShare/incude/pet
    DEPENDPATH  += $$(HOME)/BitTorrent_sync/PETCAT/!!Хотелки
}
win32 {
    DEPENDPATH  += C:/BitTorrent_sync/WorkShare/incude/pet
    DEPENDPATH  += C:/BitTorrent_sync/PETCAT/!!Хотелки
    DEPENDPATH  += C:/BitTorrent_sync/WorkShare/incude/pet
}
DEPENDPATH  += \
    $$PWD \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

#QMAKE_CXXFLAGS += -fno-show-column

QT  += network
QT  += printsupport

#DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

#DEFINES += SAVE_WIDGETS_CHECKBOX
#DEFINES += SAVE_WIDGETS_COMBOBOX
#DEFINES += SAVE_WIDGETS_PUSHBUTTON
#DEFINES += SAVE_WIDGETS_TOOLBUTTON
#DEFINES += SAVE_WIDGETS_DOUBLESPINBOX
#DEFINES += SAVE_WIDGETS_SPINBOX
#DEFINES += SAVE_WIDGETS_SLIDER
#DEFINES += SAVE_WIDGETS_SPLITTER
#DEFINES += SAVE_WIDGETS_TEXTEDIT
#DEFINES += SAVE_WIDGETS_LINEEDIT

DEFINES += ORGANIZATION_02

CONFIG(debug, debug|release) {
    DEFINES += TEST_1
    DEFINES += CREATOR
    DEFINES += IMITATOR_PET

    include (src/test/test.pri)
}

linux {
    DEFINES += TEST_2
}

DEFINES += NET_DEBUG
DEFINES += DEMO_PET
DEFINES += VIEWER
DEFINES += SCAN_HARD
#DEFINES += USE_CRC16
DEFINES += NO_TRAYICON
#DEFINES += GRAPHER_NOT_PANNING
#DEFINES += GRAPHER_NOT_ZOOM
#DEFINES += SHOW_LOCAL_ENERGY
#DEFINES += STYLE_FUSION
#DEFINES += CONTEXT_MENU

#DEFINES += LEFT_PICTURE
#DEFINES += SHOW_PICTURE

DEFINES += UPDATE_FIRMWARE
DEFINES += CALIBRATION
DEFINES += IMITATOR_BRG

DEFINES += UNDER_CONSTRUCTION

HEADERS += pet_defines.hpp

HEADERS += \
    pet.h \
    pet_bos.h \
    pet_brg.h \
    pet_td.h \
    pet_sim.h \
    pet_brg_sim.h

HEADERS += defines.hpp
SOURCES += main.cpp

include (src/mywidgets/mywaitsplashscreen.pri)
include (src/mywidgets/myprogressdialog.pri)
include (src/mywidgets/mysplashscreen.pri)
include (src/mywidgets/myfiledialog.pri)
include (src/mywidgets/mycombobox.pri)
include (src/mywidgets/mywidget.pri)

include (src/Task/Task.pri)
include (src/Other/Other.pri)
include (src/Pages/Pages.pri)

include (src/mainwindow/mainwindow.pri)
include (src/log_dock/log_dock.pri)

include (src/aboutbox/aboutbox.pri)
include (src/findpetbox/findpetbox.pri)
include (src/printbox/printbox.pri)

include (src/petcat_options/petcat_options.pri)

include (src/PET/PET.pri)

include (src/metadata/metadata.pri)

include (src/SBGlass/sbglass.pri)

include (locale.pri)

RESOURCES += \
    images/images.qrc \
    doc/doc.qrc \
    QSS/qss.qrc \
    system/system.qrc

OTHER_FILES += \
    doc/notebook.txt \
    !!Хотелки.txt

win32 {
    RC_ICONS = ico/PETCAT.ico
}

include (src/Other/turbo.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include (src/auto_inc_version.pri)
}

unix {
    DESTDIR = $$(HOME)/Programming/my_programm_bin/$$FOLDER/$$TARGET
}
win32 {
    DESTDIR = C:\BitTorrent_sync\PETCAT
}

VPATH = $$INCLUDEPATH
