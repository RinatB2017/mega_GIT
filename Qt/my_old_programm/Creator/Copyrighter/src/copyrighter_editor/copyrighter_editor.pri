#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += copyrighter_editor.hpp
SOURCES += copyrighter_editor.cpp
FORMS   += copyrighter_editor.ui

include ($$LIB_PATH/mywidgets/mytextedit.pri)
include ($$LIB_PATH2/widgets/spellcheckdialog/spellcheckdialog.pri)
include ($$LIB_PATH2/widgets/spellchecker/spellchecker.pri)
