#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

unix {
    LIBS    += -ltesseract
}

win32 {
    LIBS    += -L$$PWD/lib
    LIBS    += -llibtesseract

    INCLUDEPATH += $$PWD/win
    DEPENDPATH  += $$PWD/win
}
