#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD \
    $$PWD/PET_comm \
    $$PWD/PET_device_base \
    $$PWD/PET_device_td \
    $$PWD/PET_device_bos \
    $$PWD/PET_device_brg \
    $$PWD/PET_device_sim \
    $$PWD/PET_device_td \
    $$PWD/PET_device_brg_sim \
    $$PWD/PET
INCLUDEPATH = $$DEPENDPATH

HEADERS += PET_comm.hpp
SOURCES += PET_comm.cpp

HEADERS += PET_device_base.hpp
SOURCES += PET_device_base.cpp

###
HEADERS += PET_device_bos.hpp
SOURCES += PET_device_bos.cpp

HEADERS += PET_device_brg.hpp
SOURCES += PET_device_brg.cpp

HEADERS += PET_device_sim.hpp
SOURCES += PET_device_sim.cpp

HEADERS += PET_device_td.hpp
SOURCES += PET_device_td.cpp

HEADERS += PET_device_brg_sim.hpp
SOURCES += PET_device_brg_sim.cpp
###
