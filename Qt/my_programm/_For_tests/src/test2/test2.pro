QT += widgets testlib

HEADERS += test.hpp
SOURCES += test.cpp

LIB_PATH  = "$$PWD/../../../../lib"
LIB_PATH2 = "$$PWD/../../../../lib2"

include (../../src/config.pri)
include ($$LIB_PATH/meta/mainwindow.pri)

target.path = $$[QT_INSTALL_EXAMPLES]/qtestlib/tutorial1
INSTALLS += target
