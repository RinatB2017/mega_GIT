
QT += widgets

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    freezetablewidget.hpp
SOURCES += \
    main.cpp \
    freezetablewidget.cpp
RESOURCES += grades.qrc

VPATH = $$INCLUDEPATH
