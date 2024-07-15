#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

!contains(CONFIG, grapherbox) {
    include ($$LIB_PATH2/GrapherBox/GrapherBox.pri)
}
!contains(CONFIG, csvreader) {
    include ($$LIB_PATH2/csvreader/csvreader.pri)
}
!contains(CONFIG, qwt) {
    include ($$LIB_PATH2/qwt/qwt.pri)
}
