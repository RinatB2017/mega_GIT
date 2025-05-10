#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_QCustomPlot

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

RESOURCES += \
    data/data.qrc

win32 {
    RC_ICONS = ico/computer.ico
}

include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/QCustomPlot/qcustomplot.pri)

include (src/test_QCustomPlot_mainbox/test_QCustomPlot_mainbox.pri)

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += printsupport
}

VPATH = $$INCLUDEPATH
