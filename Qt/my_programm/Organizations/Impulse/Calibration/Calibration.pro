#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************
#27.08.13   11:07   Программу отдали на участок регулировки, им некогда, перенесли на 14:00
#27.08.13   14:00   Тестирование продолжается
#02.09.13   08:00   По просьбе участка регулировки добавлено выборочное тестирование
#03.09.13   13:20   На участок регулировки отдана новая редакция программы

#30.06.14   Новая версия программы - поиск устройств по возможности автоматически
#17.07.14   Новая версия программы - можно выбрать работу только с 1 калибратором

#09.09.14   Исправление
#              199мВ 1 кГц      - 190мВ 1 кГц
#              199мВ 50 кГц     - 190мВ 1 кГц
#              199мВ 100 кГц    - 190мВ 1 кГц

#22.09.14   Изменение кол-ва точек до 8 по всем доступным диапазонам
# Стало доступно только 20Hz, 50Hz, 100Hz, 1kHz, 10kHz, 20kHz, 50kHz, 100kHz

#калибровку сопротивлений было решено делать вручную из-за особенностей калибратора 08.08.2013

TEMPLATE = app

#CONFIG  += use_h4_7
#CONFIG  += use_h4_6
CONFIG  += use_h4_XX

use_h4_7 {
    TARGET   = Calibration_H4_7
    DEFINES += USE_ONLY_H4_7
}
use_h4_6 {
    TARGET   = Calibration_H4_6
    DEFINES += USE_ONLY_H4_6
}
use_h4_XX {
    TARGET   = Calibration_H4_XX
    DEFINES += USE_BOTH
}
#TARGET   = Calibration

FOLDER  = organizations/Impulse

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui \
    $$PWD/src/reportstring \
    $$PWD/src/myreport
INCLUDEPATH = $$DEPENDPATH

QT      += multimedia
QT      += serialport

QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

DEFINES += CONVERT_DATA_TO_ASCII

DEFINES += SAVE_WIDGETS_PROPERTIES
DEFINES += SAVE_WIDGETS_CHECKBOX
DEFINES += SAVE_WIDGETS_SPINBOX
#DEFINES += SAVE_WIDGETS_LINEEDIT

#DEFINES += R_CALIBRATION_ENABLED

DEFINES += TREE_SPLITTER

#DEFINES += NEW_POINTS
DEFINES += NEW_CALIBRATION_TIME

#DEFINES += CALIBRATOR_DEBUG


DEFINES += FAST_COMMAND

#DEFINES += USE_HTONS
DEFINES += FAKE

HEADERS += \
    src/mainbox/mainbox.hpp \
    src/defines.hpp \
    src/version.hpp \
    src/reportstring/reportstring.hpp \
    src/myreport/myreport.hpp

SOURCES += \
    src/mainbox/mainbox.cpp \
    src/main.cpp \
    src/reportstring/reportstring.cpp \
    src/myreport/myreport.cpp

#DEFINES += CALIBRATION_WIRED
#FORMS   += mainbox_wired.ui
FORMS  += mainbox.ui

RESOURCES   += \
    images/calibration.qrc \
    music/music.qrc \
    report/report.qrc

OTHER_FILES += \
    doc/notebook.txt \
    report.xml

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../../lib"
LIB_PATH2 = "../../../../lib2"

include (../common/handcontrol/handcontrol.pri)
include (src/controllog/controllog.pri)

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/crc/crc.pri)

include ($$LIB_PATH2/icons/digits.pri)

include ($$LIB_PATH2/database/database.pri)

include (../lib/measuring/multimeter_v786_2/multimeter_v786_2.pri)
include (../lib/measuring/calibrator/calibrator.pri)

include ($$LIB_PATH2/OpenRPT/OpenRPT.pri)
include ($$LIB_PATH2/connection/connection.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
