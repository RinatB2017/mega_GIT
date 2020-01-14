#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

!contains(CONFIG, grapherbox) {
    include ($$PWD/../GrapherBox/GrapherBox.pri)
}
!contains(CONFIG, csvreader) {
    include ($$PWD/../csvreader/csvreader.pri)
}
!contains(CONFIG, qwt) {
    include ($$PWD/../qwt/qwt.pri)
}
