#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = PETCat_demo

FOLDER  = organizations/Vektor

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui

unix {
    DEPENDPATH  += $$(HOME)/BitTorrent_sync/WorkShare/incude/pet
    DEPENDPATH  += $$(HOME)/BitTorrent_sync/PETCAT/!!Хотелки
    DEPENDPATH  += ../PETCat/src \
    DEPENDPATH  += ../PETCat/src/Task/pet_demo
    DEPENDPATH  += ../PETCat/src/petcat_options/src
}
win32 {
    DEPENDPATH  += E:/WorkShare/incude/pet
    DEPENDPATH  += E:/PETCAT/!!Хотелки
    DEPENDPATH  += ../PETCat/src \
    DEPENDPATH  += ../PETCat/src/Task/pet_demo
    DEPENDPATH  += ../PETCat/src/petcat_options/src
}

INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

#QT  += serialbus

DEFINES += DEMO_PET

#DEFINES += LOGO_GL
#DEFINES += FIXED_SIZE
#DEFINES += FLAG_RESIZE

DEFINES += NO_STYLETOOLBAR
#DEFINES += NO_STATUSBAR
DEFINES += NO_TRAYICON
#DEFINES += NO_TOOLBAR
#DEFINES += NO_MENU
#DEFINES += NO_LOG

#DEFINES += ONLY_ENGLISH

#DEFINES += LOG_READ_ONLY

#DEFINES += SAVE_WIDGETS_CHECKBOX
#DEFINES += SAVE_WIDGETS_COMBOBOX
#DEFINES += SAVE_WIDGETS_PUSHBUTTON
#DEFINES += SAVE_WIDGETS_TOOLBUTTON
#DEFINES += SAVE_WIDGETS_SPINBOX
#DEFINES += SAVE_WIDGETS_DOUBLESPINBOX
#DEFINES += SAVE_WIDGETS_SLIDER
#DEFINES += SAVE_WIDGETS_TEXTEDIT
#DEFINES += SAVE_WIDGETS_LINEEDIT
#DEFINES += SAVE_WIDGETS_SPLITTER

DEFINES += PROGRAMM_IN_UTF8

# только для теста
DEFINES += AUTO_TEST

HEADERS += \
    pet.h \
    pet_bos.h \
    pet_brg.h \
    pet_td.h \
    pet_sim.h \
    pet_brg_sim.h

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

win32 {
    RC_FILE = ico/myapp.rc
}

RESOURCES += images/images.qrc

OTHER_FILES += \
    doc/notebook.txt \
    !!Хотелки.txt

LIB_PATH = "../../../../lib"

include ($$LIB_PATH/qtsingleapplication/qtsingleapplication.pri)
include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/time/time.pri)
include ($$LIB_PATH/crc/crc.pri)
include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/turbo.pri)

include (../PETCat/src/Other/QtPropertyBrowser/qtpropertybrowser.pri)
include (../PETCat/src/petcat_options/petcat_options.pri)
include (../PETCat/src/Task/pet_demo/pet_demo.pri)
include (../PETCat/src/findpetbox/findpetbox.pri)

include (../PETCat/src/mywidgets/mywaitsplashscreen.pri)
include (../PETCat/src/mywidgets/myprogressdialog.pri)
include (../PETCat/src/mywidgets/mysplashscreen.pri)
include (../PETCat/src/mywidgets/myfiledialog.pri)
include (../PETCat/src/mywidgets/mywidget.pri)

include (../PETCat/src/PET/PET.pri)

CONFIG(debug, debug|release) {
    include (../PETCat/src/test/test.pri)
}

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

CONFIG(debug, debug|release) {
    message(debug mode)
}
else {
    message(release mode)
}

VPATH = $$INCLUDEPATH
