
DEPENDPATH      += $$PWD/headers
INCLUDEPATH	= $$DEPENDPATH
HEADERS += ascii_data.hpp

include (fake_serialbox5/fake_serialbox5.pri)
include (sendbox5/sendbox5.pri)
include (serialbox5/serialbox5.pri)
include (serialbox5_fix_baudrate/serialbox5_fix_baudrate.pri)
include (serialbox5_lite/serialbox5_lite.pri)
include (serialbox5_fix_baudrate_win7/serialbox5_fix_baudrate_win7.pri)

include ($$LIB_PATH2/power_knock/power_knock.pri)
