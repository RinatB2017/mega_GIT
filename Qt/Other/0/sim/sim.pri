#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    pet_brg_sim.h \
    pet_utils.h \
    pet_log.h

SOURCES	+= \
    pet_brg_sim_cmd.c \
    pet_brg_sim.c

