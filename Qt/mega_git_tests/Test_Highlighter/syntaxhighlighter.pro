QT += widgets
requires(qtConfig(filedialog))

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    highlighter.h \
    mainwindow.h

SOURCES += \
    highlighter.cpp \
    mainwindow.cpp \
    main.cpp
  
VPATH = $$INCLUDEPATH
