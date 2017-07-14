#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += b590.hpp
SOURCES += b590.cpp

use_user_calibration {
    FORMS	+= b590_user.ui
} else {
    FORMS	+= b590.ui
}

