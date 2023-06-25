
DEPENDPATH 	+= \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH	= $$DEPENDPATH

QT  *=  serialport

HEADERS += \
    text_widget.hpp \
    bin_widget.hpp \
    sendbox5.hpp
SOURCES += \
    text_widget.cpp \
    bin_widget.cpp \
    sendbox5.cpp
FORMS   += \
    text_widget.ui \
    bin_widget.ui \
    sendbox5.ui
