#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui \
    $$PWD/src/base \
    $$PWD/src/bos \
    $$PWD/src/brg \
    $$PWD/src/sim \
    $$PWD/src/td \
    $$PWD/src/brg_sim \
    $$PWD/src/udp_packet_sender
INCLUDEPATH = $$DEPENDPATH

HEADERS += pet_demo.hpp
SOURCES += pet_demo.cpp
FORMS   += pet_demo.ui

HEADERS	+= udp_packet_sender.hpp
SOURCES	+= udp_packet_sender.cpp

HEADERS += demo_device_base.hpp
SOURCES += demo_device_base.cpp

HEADERS += demo_device_bos.hpp
SOURCES += demo_device_bos.cpp

HEADERS += demo_device_brg.hpp
SOURCES += demo_device_brg.cpp

HEADERS += demo_device_sim.hpp
SOURCES += demo_device_sim.cpp

HEADERS += demo_device_td.hpp
SOURCES += demo_device_td.cpp

HEADERS += demo_device_brg_sim.hpp
SOURCES += demo_device_brg_sim.cpp

OTHER_FILES += pet_demo.txt

include (src/gen_sinus_box/gen_sinus_box.pri)

