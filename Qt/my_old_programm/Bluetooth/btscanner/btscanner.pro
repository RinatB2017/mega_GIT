TARGET = btscanner

QT = core bluetooth widgets
#requires(qtConfig(listwidget))
TEMPLATE = app

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

SOURCES = \
    main.cpp \
    device.cpp \
    service.cpp

HEADERS = \
    device.h \
    service.h

FORMS = \
    device.ui \
    service.ui

#target.path = $$[QT_INSTALL_EXAMPLES]/bluetooth/btscanner
#INSTALLS += target

VPATH = $$INCLUDEPATH
