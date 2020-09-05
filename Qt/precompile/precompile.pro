#############################################
#
# Example for using Precompiled Headers
#
#############################################

TEMPLATE  = app
LANGUAGE  = C++
#CONFIG	 += console precompile_header
#CONFIG   -= app_bundle
QT += widgets

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

# Use Precompiled headers (PCH)
CONFIG	 += precompile_header
PRECOMPILED_HEADER  = stable.h

HEADERS	  = \
    stable.h \
    mydialog.h \
    myobject.h
    
SOURCES	  = \
    main.cpp \
    mydialog.cpp \
    myobject.cpp \
    util.cpp
    
FORMS     = mydialog.ui

VPATH = $$INCLUDEPATH
