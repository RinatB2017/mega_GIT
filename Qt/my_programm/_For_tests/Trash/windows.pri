#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

win32 {
    VERSION = $${APP_MAJOR}"."$${APP_MINOR}"."$${APP_BUILD}"."$${APP_PATCH}

    QMAKE_TARGET_COMPANY = Home
    QMAKE_TARGET_PRODUCT = $$TARGET
    QMAKE_TARGET_COPYRIGHT = "Copyright (c) 2020-2025"
    QMAKE_TARGET_DESCRIPTION = "my description"

    RC_ICONS = ico/computer.ico # нужно, чтобы создался фал .rc автоматом
    # RC_FILE  = programm.rc
}
