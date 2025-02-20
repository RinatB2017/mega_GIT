#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

greaterThan(QT_MAJOR_VERSION, 4) {
    QT *= webenginewidgets
    # QT *= webengine
} else {
    QT *= webkit
}
QT  *= network

HEADERS += mybrowser.hpp
SOURCES += mybrowser.cpp
FORMS   += mybrowser.ui

HEADERS += custompage.h

include ($$LIB_PATH2/QXmlPutGet/QXmlPutGet.pri)
include ($$LIB_PATH2/web/webattributedialog/webattributedialog.pri)
