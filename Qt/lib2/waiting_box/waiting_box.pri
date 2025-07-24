
DEPENDPATH 	+= \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH	= $$DEPENDPATH

HEADERS += waiting_box.hpp
SOURCES += waiting_box.cpp
FORMS   += waiting_box.ui

RESOURCES += \
    $$PWD/images/waiting_box.qrc
