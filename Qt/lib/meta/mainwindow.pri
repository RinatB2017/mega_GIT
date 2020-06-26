#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

CONFIG(debug, debug|release) {
} else {
    DEFINES += NO_LOG_DEBUG
    DEFINES += NO_LOG_TRACE
}

include ($$PWD/../qtsingleapplication/qtsingleapplication.pri)
include ($$PWD/../mywidgets/mywaitsplashscreen.pri)
include ($$PWD/../mywidgets/mysplashscreen.pri)

include ($$PWD/../mywidgets/mysettings.pri)
include ($$PWD/../mywidgets/mymessages.pri)
include ($$PWD/../mywidgets/mywidget.pri)

include ($$PWD/../time/time.pri)
include ($$PWD/../locale.pri)
include ($$PWD/../mw/mw.pri)
include ($$PWD/../turbo.pri)
