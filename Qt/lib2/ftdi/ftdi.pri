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

# FTDI библиотеку подключать не обязательно - код встроен в приложение
