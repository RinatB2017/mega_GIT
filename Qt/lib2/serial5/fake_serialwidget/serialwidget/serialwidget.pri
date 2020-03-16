
DEPENDPATH 	+= $$PWD/src
INCLUDEPATH	= $$DEPENDPATH

HEADERS += serialwidget.hpp
SOURCES += serialwidget.cpp

use_worker {
} else {
    include (../worker_fake/worker_fake.pri)
}
