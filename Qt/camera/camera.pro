TEMPLATE    = app
TARGET      = camera

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

QT  +=  multimedia multimediawidgets
QT  +=  widgets

HEADERS = \
    imagesettings.h \
    videosettings.h \
    camera.h \

SOURCES = \
    imagesettings.cpp \
    videosettings.cpp \
    camera.cpp \
    main.cpp

FORMS += \
    imagesettings.ui \
    videosettings.ui \
    camera.ui

RESOURCES += images/camera.qrc

VPATH = $$INCLUDEPATH
