#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

!contains(CONFIG, histogrambox) {
    include ($$PWD/../HistogramBox/HistogramBox.pri)
}
!contains(CONFIG, csvreader) {
    include ($$PWD/../csvreader/csvreader.pri)
}
!contains(CONFIG, qwt) {
    include ($$PWD/../qwt/qwt.pri)
}
