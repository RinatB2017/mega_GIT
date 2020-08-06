#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

greaterThan(QT_MAJOR_VERSION, 4) {
    QT *= webenginewidgets
} else {
    QT *= webkit
}
QT  *= network

HEADERS += mybrowser.hpp
SOURCES += mybrowser.cpp
FORMS   += mybrowser.ui

HEADERS += custompage.h

include ($$LIB_PATH2/QXmlPutGet/qxmlputget.pri)
