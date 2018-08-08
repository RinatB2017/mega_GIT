#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_OpenGL

FOLDER  = tests

PROGRAMM_PATH  += \
    $$PWD/src
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

#QMAKE_CXXFLAGS += -fno-show-column
QT      += opengl
LIBS    += -lglut -lGLU

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += PROGRAMM_IN_UTF8

DEFINES += SAVE_WIDGETS_DOUBLESPINBOX

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

win32 {
    RC_FILE = ico/myapp.rc
}

RESOURCES += \
    images/images.qrc \
    doc/doc.qrc

OTHER_FILES += doc/notebook.txt

#----------------------------------------------
LIB_PATH = "../../../Qt/lib"
include ($$LIB_PATH/meta/mainwindow.pri)

include (src/test_opengl_mainbox/test_opengl_mainbox.pri)
include (src/test_qgl_widget/test_qgl_widget.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}
#----------------------------------------------

VPATH = $$INCLUDEPATH
