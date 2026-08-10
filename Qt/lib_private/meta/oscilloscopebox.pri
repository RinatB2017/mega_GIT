#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

!contains(CONFIG, oscilloscopebox) {
    include ($$PWD/../OscilloscopeBox/OscilloscopeBox.pri)
}
!contains(CONFIG, csvreader) {
    include ($$PWD/../csvreader/csvreader.pri)
}
!contains(CONFIG, qwt) {
    include ($$PWD/../qwt/qwt.pri)
}
