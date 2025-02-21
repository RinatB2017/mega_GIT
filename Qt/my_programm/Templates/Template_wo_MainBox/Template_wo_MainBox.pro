#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Template_wo_MainBox

FOLDER  = _Templates

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/newmainwindow
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    newmainwindow.hpp \
    defines.hpp

SOURCES += \
    newmainwindow.cpp \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    images/images.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

#----------------------------------------------
win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/meta/grapherbox.pri)

greaterThan(QT_MAJOR_VERSION, 4) {
    include ($$LIB_PATH2/serial5/serialwidget/serialwidget.pri)
    include ($$LIB_PATH2/serial5/serial5.pri)
} esle {
    include ($$LIB_PATH2/serial4/serial4.pri)
}
#----------------------------------------------

VPATH = $$INCLUDEPATH
