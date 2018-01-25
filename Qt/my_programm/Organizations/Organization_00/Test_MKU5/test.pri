
DEPENDPATH  += \
    $$PWD/test
INCLUDEPATH = $$DEPENDPATH

HEADERS     += \
    test_rs232_1.hpp \
    test_rs232_2.hpp \
    test_rs232_3.hpp \
    test_flbi_1.hpp \
    test_flbi_2.hpp \
    test_flbi_3.hpp \
    test_thread.hpp \
    proto.hpp \
    property_editor.hpp \
    r4funcs.hpp \
    r4defs.hpp \
    delegates.hpp

SOURCES     += \
    test_rs232_1.cpp \
    test_rs232_2.cpp \
    test_rs232_3.cpp \
    test_flbi_1.cpp \
    test_flbi_2.cpp \
    test_flbi_3.cpp \
    test_thread.cpp \
    proto.cpp \
    property_editor.cpp \
    r4funcs.cpp
