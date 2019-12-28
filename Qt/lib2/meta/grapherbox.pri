#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

!contains(CONFIG, grapherbox) {
    include (../GrapherBox/GrapherBox.pri)
}
!contains(CONFIG, csvreader) {
    include (../csvreader/csvreader.pri)
}
!contains(CONFIG, qwt) {
    include (../qwt/qwt.pri)
}

include ($$LIB_PATH/mywidgets/myfiledialog.pri)
