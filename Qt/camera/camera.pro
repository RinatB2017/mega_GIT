TEMPLATE = app
TARGET = camera


DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

QT  += multimedia multimediawidgets
QT  +=widgets

HEADERS = \
    camera.h \
    imagesettings.h \
    videosettings.h

SOURCES = \
    main.cpp \
    camera.cpp \
    imagesettings.cpp \
    videosettings.cpp

FORMS += \
    camera.ui \
    videosettings.ui \
    imagesettings.ui

RESOURCES += images/camera.qrc

VPATH = $$INCLUDEPATH
