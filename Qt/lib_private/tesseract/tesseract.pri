#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -llibtesseract
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -llibtesseractd
#else:unix:!macx: LIBS += -ltesseract

#win32 {
#    INCLUDEPATH += $$PWD
#    DEPENDPATH  += $$PWD
#}

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/lib/liblibtesseract.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/lib/liblibtesseractd.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/lib/libtesseract.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/lib/libtesseractd.lib
#else:unix:!macx: PRE_TARGETDEPS += $$PWD/lib/liblibtesseract.a

# если в проекте есть и статические и динамические библиотеки, то не надо добавлять -l в LIBS для win
win32 {
    INCLUDEPATH += $$PWD
    DEPENDPATH  += $$PWD

    CONFIG(debug, debug|release) {
        LIBS += $$PWD/lib/libtesseractd.lib
    } else {
        PRE_TARGETDEPS += $$PWD/lib/libtesseract.lib
#        LIBS += $$PWD/lib/libtesseract.lib
#        $$PWD/lib/libtesseract.lib
    }
}

unix {
    LIBS += -ltesseract
}

