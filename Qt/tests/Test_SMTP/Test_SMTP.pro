#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = Test_SMTP

FOLDER  = old_programm

PROGRAMM_PATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/smtp \
    $$PWD/src/sendemail \
    $$PWD/src/sendemail/ui
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

QMAKE_CXXFLAGS += -fno-show-column

QT  += network

#DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8

DEFINES += NO_TRAYICON
#DEFINES += NO_LOG

HEADERS += \
    emailaddress.h \
    mimeattachment.h \
    mimecontentformatter.h \
    mimefile.h \
    mimehtml.h \
    mimeinlinefile.h \
    mimemessage.h \
    mimemultipart.h \
    mimepart.h \
    mimetext.h \
    quotedprintable.h \
    smtpclient.h \
    smtpexports.h

SOURCES += \
    main.cpp \
    emailaddress.cpp \
    mimeattachment.cpp \
    mimecontentformatter.cpp \
    mimefile.cpp \
    mimehtml.cpp \
    mimeinlinefile.cpp \
    mimemessage.cpp \
    mimemultipart.cpp \
    mimepart.cpp \
    mimetext.cpp \
    quotedprintable.cpp \
    smtpclient.cpp

win32 {
    RC_FILE = ico/myapp.rc
}

HEADERS += sendemail.hpp
SOURCES += sendemail.cpp
FORMS   += sendemail.ui

LIB_PATH = "../../../Qt/lib"

include ($$LIB_PATH/turbo.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
