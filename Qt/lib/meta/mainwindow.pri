#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

include ($$PWD/../qtsingleapplication/qtsingleapplication.pri)
include ($$PWD/../mywidgets/mywaitsplashscreen/mywaitsplashscreen.pri)
include ($$PWD/../mywidgets/mysplashscreen/mysplashscreen.pri)

include ($$PWD/../mywidgets/mysettings/mysettings.pri)
include ($$PWD/../mywidgets/mymessages/mymessages.pri)
include ($$PWD/../mywidgets/mywidget/mywidget.pri)

include ($$PWD/../time/time.pri)
include ($$PWD/../locale.pri)
include ($$PWD/../mw/mw.pri)
include ($$PWD/../turbo.pri)

include ($$LIB_PATH/mywidgets/timer_messagebox/timer_messagebox.pri)
include ($$LIB_PATH/mywidgets/timer_options/timer_options.pri)
include ($$LIB_PATH/mywidgets/digital_clock/digital_clock.pri)
include ($$LIB_PATH/mywidgets/lcd_clock/lcd_clock.pri)

include ($$LIB_PATH2/simplehttpserver/simplehttpserver.pri)

include ($$LIB_PATH3/docker_mover/docker_mover.pri)

RESOURCES += \
    locale/lang.qrc
