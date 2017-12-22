#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE  = subdirs
CONFIG   *= ordered

SUBDIRS += \
    Aggregator \
    CalcMd5 \
    Double_browser \
    Platanov_blowser \
    ProxyWalker \
    ProxyWalker_M \
    WebKit \
    WebKit_old
    
greaterThan(QT_MAJOR_VERSION, 4) {
    SUBDIRS += Copyrighter
}

