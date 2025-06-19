#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/include
INCLUDEPATH = $$DEPENDPATH

unix {
    LIBS    += -L$$PWD/linux_lib
    LIBS    += -llimereport
    LIBS    += -lQtZint
}
win32 {
    # HEADERS += \
    #     lrcallbackdatasourceintf.h \
    #     lrdatasourceintf.h \
    #     lrdatasourcemanagerintf.h \
    #     lrglobal.h \
    #     lrpreparedpagesintf.h \
    #     lrpreviewreportwidget.h \
    #     lrrenderengine.h \
    #     lrreportdesignwindowintrerface.h \
    #     lrreportengine.h \
    #     lrscriptenginemanagerintf.h
    # SOURCES += \
    #     lrglobal.cpp

    LIBS    += -L$$PWD/win_lib
    LIBS    += -llimereport
    LIBS    += -lQtZint
}
