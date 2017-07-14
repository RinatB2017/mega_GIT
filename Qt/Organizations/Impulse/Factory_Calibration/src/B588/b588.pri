#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += b588.hpp
SOURCES += b588.cpp

use_user_calibration {
    FORMS	+= b588_user.ui
} else {
    FORMS	+= b588.ui
}
