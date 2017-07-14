#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD \
    $$PWD/src/serialbox \
    $$PWD/src/serialbox/ui \
    $$PWD/src/fakeenumerator \
    $$PWD/src/fakeserial \
    $$PWD/src/emulator_v786_2
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    src/serialbox/sendbox.hpp \
    src/serialbox/serialbox.hpp \
    src/fakeenumerator/fakeenumerator.hpp \
    src/fakeserial/fakeserial.hpp \
    src/emulator_v786_2/emulator_v786_2.hpp

SOURCES += \
    src/serialbox/sendbox.cpp \
    src/serialbox/serialbox.cpp \
    src/fakeenumerator/fakeenumerator.cpp \
    src/fakeserial/fakeserial.cpp \
    src/emulator_v786_2/emulator_v786_2.cpp

FORMS += src/serialbox/ui/serialbox.ui

include (../power_knock/power_knock.pri)
