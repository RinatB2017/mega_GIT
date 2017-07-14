#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

RESOURCES += navigator/data.qrc

include (receiver.pri)
include (tester.pri)
include (worker.pri)

LIB_PATH = "../../../lib"
include ($$LIB_PATH/proto_NMEA_0183/proto_NMEA_0183.pri)

