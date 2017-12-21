
DEPENDPATH += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

LIBS    += -lopencv_core
LIBS    += -lopencv_highgui
LIBS    += -lopencv_objdetect
LIBS    += -lopencv_ocl

SOURCES += QOpenCVWidget.cpp
HEADERS += QOpenCVWidget.hpp
