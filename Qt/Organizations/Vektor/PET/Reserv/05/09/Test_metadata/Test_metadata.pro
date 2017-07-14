#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_metadata

FOLDER  = organizations/Vektor

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp \
    mainbox.hpp

SOURCES += \
    mainbox.cpp \
    main.cpp

FORMS   += mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

RESOURCES += \
    images/images.qrc \
    doc/doc.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

LIB_PATH = "../../../../lib"

include ($$LIB_PATH/meta/mainwindow.pri)

include (../PETCat/src/mywidgets/myfiledialog.pri)
include (../PETCat/src/mywidgets/mycombobox.pri)

include (../PETCat/src/metadata/metadata.pri)


!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
