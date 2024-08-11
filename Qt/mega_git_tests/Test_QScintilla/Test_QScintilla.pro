#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_QScintilla

FOLDER  = tests

include (conf/conf.pri)

PROGRAMM_PATH  += \
    $$PWD/src
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

unix {
    LIBS    += -lqscintilla2_qt5
}

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    data/data.qrc \
    ico/icons.qrc \
    images/images.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

#----------------------------------------------
include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)

include ($$LIB_PATH/meta/mainwindow.pri)

win32 {
    include ($$LIB_PATH2/QScintilla/QScintilla.pri)
}
include (src/qscintilla_mainbox/qscintilla_mainbox.pri)
#----------------------------------------------

VPATH = $$INCLUDEPATH
