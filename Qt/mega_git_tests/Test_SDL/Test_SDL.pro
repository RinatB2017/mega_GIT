
TEMPLATE = app
TARGET   = Test_SDL

DEPENDPATH += \
    /usr/include/SDL \
    .
INCLUDEPATH = $$DEPENDPATH

SOURCES += main.cpp

LIBS    += -lSDL
LIBS    += -lSDL_image
LIBS    += -lSDL_ttf
LIBS    += -lSDL_mixer

unix {
    QMAKE_CFLAGS    += -std=c++11
    QMAKE_CXXFLAGS  += -std=c++11
    QMAKE_LFLAGS    += -std=c++11
}

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    message(Qt5 = $$QT)
}
else {
    message(Qt4 = $$QT)
}
