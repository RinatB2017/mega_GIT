
TEMPLATE    = app
TARGET      = PacketSender

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

QT  += core gui network widgets

unix:!macx {
    OBJECTS_DIR = /dev/shm/my_programm/$$TARGET/obj
    MOC_DIR     = /dev/shm/my_programm/$$TARGET/moc
    UI_DIR      = /dev/shm/my_programm/$$TARGET/ui
    RCC_DIR     = /dev/shm/my_programm/$$TARGET/rc
}

win32 {
    TEMP_PATH = "C:\\temp"
    OBJECTS_DIR = $$TEMP_PATH\\my_programm\\$$TARGET\\obj
    MOC_DIR     = $$TEMP_PATH\\my_programm\\$$TARGET\\moc
    UI_DIR      = $$TEMP_PATH\\my_programm\\$$TARGET\\ui
    RCC_DIR     = $$TEMP_PATH\\my_programm\\$$TARGET\\rc
}

unix {
    DESTDIR = $$(HOME)/Programming/my_programm_bin/$$TARGET
}
win32 {
    DESTDIR = C:/Programming/my_programm_bin/$$TARGET
}

greaterThan(QT_MAJOR_VERSION, 4) : QT += widgets

#Needed for Qt5.5 with El Capitan
QMAKE_MAC_SDK = macosx10.11

#enable only if compiling .com for windows
#win32:CONFIG += console
#win32:DEFINES += CONSOLE_BUILD

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    packetnetwork.cpp \
    packet.cpp \
    sendpacketbutton.cpp \
    brucethepoodle.cpp \
    tcpthread.cpp \
    persistentconnection.cpp \
    settings.cpp \
    about.cpp \
    subnetcalc.cpp

HEADERS  += \
    mainwindow.h \
    packetnetwork.h \
    packet.h \
    globals.h \
    sendpacketbutton.h \
    brucethepoodle.h \
    tcpthread.h \
    persistentconnection.h \
    settings.h \
    about.h \
    subnetcalc.h \
    ebcdic_ascii_map.h

FORMS    += \
    mainwindow.ui \
    brucethepoodle.ui \
    persistentconnection.ui \
    settings.ui \
    about.ui \
    subnetcalc.ui

macx:CONFIG += app_bundle

OTHER_FILES += \
    packetsender.css \
    packetsender_mac.css

win32:RC_FILE = psicon.rc

macx:ICON = psicons.icns

RESOURCES += images/packetsender.qrc

VPATH = $$INCLUDEPATH
