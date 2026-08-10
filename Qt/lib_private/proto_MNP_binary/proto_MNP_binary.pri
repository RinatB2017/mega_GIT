#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += proto_MNP_binary.hpp
SOURCES += proto_MNP_binary.cpp

LIB_PATH = "../lib"
include ($$LIB_PATH/QHexEdit/qhexedit.pri)
