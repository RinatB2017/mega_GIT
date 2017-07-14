
DEPENDPATH += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/qt4support/src \
    $$PWD/src/qt4support/include/private \
    $$PWD/src/qt4support/include/QtCore
INCLUDEPATH += \
    $$PWD/. \
    $$PWD/src \
    $$PWD/src/qt4support/include/QtCore \
    $$PWD/src/qt4support/include/private

HEADERS += \
    src/qserialport.h \
    src/qserialport_p.h \
#    src/qserialport_symbian_p.h \
    src/qserialport_unix_p.h \
#    src/qserialport_win_p.h \
    src/qserialportglobal.h \
    src/qserialportinfo.h \
    src/qserialportinfo_p.h \
    src/qtudev_p.h \
    src/qt4support/include/private/qlockfile_p.h \
    src/qt4support/include/private/qringbuffer_p.h \
    src/qt4support/include/QtCore/qlockfile.h \
    src/qt4support/include/QtCore/qwineventnotifier.h
           
SOURCES += \
    src/qserialport.cpp \
#    src/qserialport_symbian.cpp \
    src/qserialport_unix.cpp \
#    src/qserialport_win.cpp \
#    src/qserialport_wince.cpp \
    src/qserialportinfo.cpp \
#    src/qserialportinfo_mac.cpp \
#    src/qserialportinfo_symbian.cpp \
    src/qserialportinfo_unix.cpp \
#    src/qserialportinfo_win.cpp \
#    src/qserialportinfo_wince.cpp \
    src/qt4support/src/qlockfile.cpp \
    src/qt4support/src/qlockfile_unix.cpp
