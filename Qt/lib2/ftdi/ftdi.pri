#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
  ftdi.h \
  ftdi_i.h \
  ftdi_version_i.h \
  ftd2xx.h
  
SOURCES += ftdi.c

#FIXME надо нормально работать с libFTDI
win32 {
    LIBS    += -L$$PWD/lib
    LIBS    += -lsetupapi
#    LIBS    += -lftdi1
}
