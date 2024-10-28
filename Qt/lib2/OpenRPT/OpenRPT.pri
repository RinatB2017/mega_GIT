#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

greaterThan(QT_MAJOR_VERSION, 5) {
    include (qt6/qt6.pri)
} else {
    greaterThan(QT_MAJOR_VERSION, 4) {
        include (qt5/qt5.pri)
    } else {
            include (qt4/qt4.pri)
        }
}
INCLUDEPATH = $$DEPENDPATH
