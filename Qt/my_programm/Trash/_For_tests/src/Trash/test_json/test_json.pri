#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += test_json.hpp
SOURCES += test_json.cpp
FORMS   += test_json.ui

OTHER_FILES += $$PWD/doc/notebook.txt

include ($$LIB_PATH2/widgets/showjson/showjson.pri)
