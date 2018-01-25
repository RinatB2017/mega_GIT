#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

include (log/log.pri)
include (log_options/log_options.pri)
include (qtsingleapplication/qtsingleapplication.pri)
include (QtPropertyBrowser/qtpropertybrowser.pri)
include (helpbrowser/helpbrowser.pri)
include (GrapherBox/GrapherBox.pri)
include (csvreader/csvreader.pri)
include (time/time.pri)
include (crc/crc.pri)
include (qwt/qwt.pri)

include (dat3/dat3.pri)
include (viewer/viewer.pri)

QT  += serialport network
QT  += xml

include (MKT/MKT.pri)
include (AD9106_box/AD9106_box.pri)

CONFIG(debug, debug|release) {
    DEFINES += MKT
    DEFINES += SIMULATOR
}
