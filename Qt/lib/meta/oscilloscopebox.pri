#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

!contains(CONFIG, oscilloscopebox) {
    include (../OscilloscopeBox/OscilloscopeBox.pri)
}
!contains(CONFIG, csvreader) {
    include (../csvreader/csvreader.pri)
}
!contains(CONFIG, qwt) {
    include (../qwt/qwt.pri)
}
