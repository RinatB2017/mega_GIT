
DEPENDPATH  += $$PWD/headers
INCLUDEPATH	= $$DEPENDPATH
HEADERS += ascii_data.hpp

include (sendbox5/sendbox5.pri)
include (serialbox5/serialbox5.pri)
include (serialbox5_fix_baudrate/serialbox5_fix_baudrate.pri)
include (serialbox5_lite/serialbox5_lite.pri)
include (serialbox5_wo_form/serialbox5_wo_form.pri)

include ($$LIB_PATH2/power_knock/power_knock.pri)
include ($$LIB_PATH2/icons/digits.pri)
include ($$LIB_PATH2/icons/arrows.pri)
