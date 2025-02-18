#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

unix:!macx {
    OBJECTS_DIR = /dev/shm/my_programm/$$FOLDER/$$TARGET/obj
    MOC_DIR     = /dev/shm/my_programm/$$FOLDER/$$TARGET/moc
    UI_DIR      = /dev/shm/my_programm/$$FOLDER/$$TARGET/ui
    RCC_DIR     = /dev/shm/my_programm/$$FOLDER/$$TARGET/rc
}

macx {
    OBJECTS_DIR = build/obj
    MOC_DIR     = build/moc
    UI_DIR      = build/ui
    RCC_DIR     = build/rc
}

win32 {
    TEMP_PATH = "C:\\shm"
    #TEMP_PATH = "K:"
    OBJECTS_DIR = $$TEMP_PATH\\my_programm\\$$FOLDER\\$$TARGET\\obj
    MOC_DIR     = $$TEMP_PATH\\my_programm\\$$FOLDER\\$$TARGET\\moc
    UI_DIR      = $$TEMP_PATH\\my_programm\\$$FOLDER\\$$TARGET\\ui
    RCC_DIR     = $$TEMP_PATH\\my_programm\\$$FOLDER\\$$TARGET\\rc

    CONFIG -= debug_and_release #debug_and_release_target
    CONFIG += no_fixpath
}
###############################################################################
unix:!macx {
    DESTDIR = $$(HOME)/Programming/my_programm_bin/$$FOLDER/$$TARGET
}
macx {
    DESTDIR = bin
}
win32 {
    DESTDIR = C:/Programming/my_programm_bin/$$FOLDER/$$TARGET
}
###############################################################################
#управление оптимизацией компилятора
#OPTIMIZE = -pipe -O0 #no optimization
#OPTIMIZE = -pipe -O1 #code size, execution time
#OPTIMIZE = -pipe -O2 #code size, exectuion time, performance of generated code
#OPTIMIZE = -pipe -O3 #as 2 and some more
#OPTIMIZE = -pipe -Os #only size code optimization
#OPTIMIZE = -pipe -Ofast #only for gcc-4.6

unix:!macx {
    CONFIG(debug, debug|release) {
        OPTIMIZE = -O0
    }
    else {
        OPTIMIZE = -O2
    }
    linux {
        OPTIMIZE += -pipe
    }
    QMAKE_CFLAGS   += $${OPTIMIZE}
    QMAKE_CXXFLAGS += $${OPTIMIZE}
    QMAKE_LFLAGS   += $${OPTIMIZE}
    QMAKE_OBJECTIVE_CFLAGS += $${OPTIMIZE}

    #OPTIMIZE    += -Wno-missing-braces -Wno-missing-field-initializers
    QMAKE_CXX   = ccache g++

    #CONFIG += warn_off
    #QMAKE_CXXFLAGS += -Wall
    #QMAKE_CFLAGS_WARN_ON   -= -Wno-missing-braces -Wno-missing-field-initializers
    #QMAKE_CXXFLAGS_WARN_ON -= -Wno-missing-braces -Wno-missing-field-initializers
}
###############################################################################
QMAKE_CFLAGS   += $${OPTIMIZE}
QMAKE_CXXFLAGS += $${OPTIMIZE}
QMAKE_LFLAGS   += $${OPTIMIZE}
QMAKE_OBJECTIVE_CFLAGS += $${OPTIMIZE}
###############################################################################
greaterThan(QT_MAJOR_VERSION, 5) {
    QT += core5compat widgets
    DEFINES += HAVE_QT5
    message(Qt5 = $$QT)
} else {
    DEFINES += HAVE_QT4
    message(Qt4 = $$QT)
}
###############################################################################
