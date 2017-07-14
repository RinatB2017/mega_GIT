#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_MKT_box

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

QT  += xml
QT  += serialport

DEFINES += DEMO_PET
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += PROGRAMM_IN_UTF8
DEFINES += SAVE_WIDGETS_SPINBOX
DEFINES += SAVE_WIDGETS_DOUBLESPINBOX

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

include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/time/time.pri)
include ($$LIB_PATH/crc/crc.pri)
include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/turbo.pri)

include (../PETCat/src/Other/qtsingleapplication/qtsingleapplication.pri)
include (../PETCat/src/petcat_options/petcat_options.pri)

include (../PETCat/src/mywidgets/mywaitsplashscreen.pri)
include (../PETCat/src/mywidgets/myprogressdialog.pri)
include (../PETCat/src/mywidgets/mysplashscreen.pri)
include (../PETCat/src/mywidgets/myfiledialog.pri)
include (../PETCat/src/mywidgets/mywidget.pri)

include (../PETCat/src/findpetbox/findpetbox.pri)
include (../PETCat/src/Task/pet_demo/pet_demo.pri)
include (../PETCat/src/Other/GrapherBox/GrapherBox.pri)
include (../PETCat/src/Other/csvreader/csvreader.pri)
include (../PETCat/src/Other/qwt/qwt.pri)
include (../PETCat/src/Other/MKT/MKT.pri)
include (../PETCat/src/PET/PET.pri)

CONFIG(debug, debug|release) {
    include (../PETCat/src/test/test.pri)
}

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

unix {
    DESTDIR = $$(HOME)/Programming/my_programm_bin/$$FOLDER/$$TARGET
}
win32 {
    #DESTDIR = C:\BitTorrent_sync\PETCAT
    DESTDIR = C:\BitTorrent_sync\USER\Rinat\PETCAT
}

VPATH = $$INCLUDEPATH
