#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = Test_SMTP

FOLDER  = tests

include (conf/conf.pri)

PROGRAMM_PATH  += \
    $$PWD/src
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

QT  += network

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH = "$$PWD/../../../Qt/lib"

include ($$LIB_PATH/turbo.pri)
include (src/sendemail/sendemail.pri)
include (src/smtp/smtp.pri)






VPATH = $$INCLUDEPATH
