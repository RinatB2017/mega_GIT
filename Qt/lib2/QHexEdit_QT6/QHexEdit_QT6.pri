#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    qhexeditcomments.h \
    qhexeditdatareader.h \
    qhexedithighlighter.h \
    qhexeditdatadevice.h \
    qhexeditdatawriter.h \
    qhexeditprivate.h \
    qhexeditdata.h \
    qhexedit.h \
    sparserangemap.h \
    doublehexeditor.hpp


SOURCES += \
    qhexeditcomments.cpp \
    qhexeditdatadevice.cpp \
    qhexedithighlighter.cpp \
    qhexedit.cpp \
    qhexeditdatareader.cpp \
    qhexeditprivate.cpp \
    qhexeditdata.cpp \
    qhexeditdatawriter.cpp \
    sparserangemap.cpp \
    doublehexeditor.cpp


OTHER_FILES += \
    LICENSE \
    README.md

greaterThan(QT_MAJOR_VERSION, 4) {
    unix {
        QMAKE_CXXFLAGS += -std=c++11
    }
}
