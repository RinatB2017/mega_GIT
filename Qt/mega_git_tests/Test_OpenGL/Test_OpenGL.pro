#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_OpenGL

FOLDER  = tests

include (conf/conf.pri)

PROGRAMM_PATH  += \
    $$PWD/src
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

QT      += opengl
LIBS    += -lglut -lGLU

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

RESOURCES += \
    images/images.qrc

OTHER_FILES += doc/notebook.txt

#----------------------------------------------
include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)

include ($$LIB_PATH/meta/mainwindow.pri)

include (src/test_opengl_mainbox/test_opengl_mainbox.pri)
include (src/test_qgl_widget/test_qgl_widget.pri)

#----------------------------------------------

VPATH = $$INCLUDEPATH
