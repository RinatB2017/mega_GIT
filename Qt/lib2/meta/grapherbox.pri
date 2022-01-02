#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

!contains(CONFIG, grapherbox) {
    include ($$PWD/../GrapherBox/GrapherBox.pri)
}
!contains(CONFIG, csvreader) {
    include ($$PWD/../csvreader/csvreader.pri)
}

# собираем qwt прямо из кода
!contains(CONFIG, qwt) {
#    DEPENDPATH  += $$PWD/../qwt/include # надо исправить
#    INCLUDEPATH += $$PWD/../qwt/include # надо исправить

    include ($$PWD/../qwt/qwt.pri)
}

# подключаем библиотеки
#unix {
#    LIBS    += -lqwt


#win32 {
#    message (need QWT libs)
#    DEFINES += QT_DLL QWT_DLL
#    LIBS    += -L$$PWD/../qwt/lib
#    LIBS    += -lqwt
#    LIBS    += -lqwtmathml

