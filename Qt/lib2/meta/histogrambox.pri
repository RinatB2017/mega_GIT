#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

!contains(CONFIG, histogrambox) {
    include (../HistogramBox/HistogramBox.pri)
}
!contains(CONFIG, csvreader) {
    include (../csvreader/csvreader.pri)
}
!contains(CONFIG, qwt) {
    include (../qwt/qwt.pri)
}
