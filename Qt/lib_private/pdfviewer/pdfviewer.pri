#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src 
INCLUDEPATH = $$DEPENDPATH

QT += webenginewidgets

HEADERS += \
    qpdfwidget.h \
    pdfjsbridge.h \
    qpdf.h
    
SOURCES += \
    qpdfwidget.cpp \
    pdfjsbridge.cpp

RESOURCES   += \
    $$PWD/pdfview.qrc
