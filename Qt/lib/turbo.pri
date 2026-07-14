#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

# https://stackoverflow.com/questions/36871334/qmake-conditional-for-raspberrypi
linux {
    contains(QMAKE_HOST.arch, arm.*):{
        message(raspberry)
        MOC_DIR         = temp/moc
        UI_DIR          = temp/ui
        UI_HEADERS_DIR  = temp/ui
        UI_SOURCES_DIR  = temp/ui
        OBJECTS_DIR     = temp/obj
        RCC_DIR         = temp/rc

        QMLCACHE_DIR    = temp/qmlcache
    }else{
        # message(linux)
        MOC_DIR         = /dev/shm/my_programm/$$FOLDER/$$TARGET/moc
        UI_DIR          = /dev/shm/my_programm/$$FOLDER/$$TARGET/ui
        UI_HEADERS_DIR  = /dev/shm/my_programm/$$FOLDER/$$TARGET/ui
        UI_SOURCES_DIR  = /dev/shm/my_programm/$$FOLDER/$$TARGET/ui
        OBJECTS_DIR     = /dev/shm/my_programm/$$FOLDER/$$TARGET/obj
        RCC_DIR         = /dev/shm/my_programm/$$FOLDER/$$TARGET/rc

        QMLCACHE_DIR    = /dev/shm/my_programm/$$FOLDER/$$TARGET/qmlcache
        QMAKE_CLEAN     += *_qmlcache.qrc
    }
}

macx {
    MOC_DIR         = build/moc
    UI_DIR          = build/ui
    UI_HEADERS_DIR  = build/ui
    UI_SOURCES_DIR  = build/ui
    OBJECTS_DIR     = build/obj
    RCC_DIR         = build/rc
}

android {
    MOC_DIR         = build/moc
    UI_DIR          = build/ui
    UI_HEADERS_DIR  = build/ui
    UI_SOURCES_DIR  = build/ui
    OBJECTS_DIR     = build/obj
    RCC_DIR         = build/rc
}
###############################################################################
unix:!macx {
    DESTDIR = $$(HOME)/Programming/my_programm_bin/$$FOLDER/$$TARGET
}
macx {
    DESTDIR = bin
}

win32 {
    BIN_PATH  = "C:/temp/bin"
    # TEMP_PATH = "X:/temp/obj"
    TEMP_PATH = "W:/temp/obj"
    # в XP нет смысла делать виртуальный диск в системе

    MOC_DIR         = $$TEMP_PATH/my_programm/$$FOLDER/$$TARGET/moc
    UI_DIR          = $$TEMP_PATH/my_programm/$$FOLDER/$$TARGET/ui
    UI_HEADERS_DIR  = $$TEMP_PATH/my_programm/$$FOLDER/$$TARGET/ui
    UI_SOURCES_DIR  = $$TEMP_PATH/my_programm/$$FOLDER/$$TARGET/ui
    OBJECTS_DIR     = $$TEMP_PATH/my_programm/$$FOLDER/$$TARGET/obj
    RCC_DIR         = $$TEMP_PATH/my_programm/$$FOLDER/$$TARGET/rc

    CONFIG -= debug_and_release #debug_and_release_target
    CONFIG += no_fixpath

    DEFINES += WIN32_LEAN_AND_MEAN

    contains(QT_ARCH, i386) {
        message("32-bit")
        CONFIG(debug, debug|release) {
            DESTDIR = $$BIN_PATH/$$FOLDER/Win32/debug
        }
        else {
            DESTDIR = $$BIN_PATH/$$FOLDER/Win32/release
        }
    } else {
        message("64-bit")
        win32-g++ {
            message("mingw")
            CONFIG(debug, debug|release) {
                DESTDIR = $$BIN_PATH/$$FOLDER/Win64/mingw/debug
            }
            else {
                DESTDIR = $$BIN_PATH/$$FOLDER/Win64/mingw/release
            }
        }
        win32-msvc*{
            message("msvc")
            CONFIG(debug, debug|release) {
                DESTDIR = $$BIN_PATH/$$FOLDER/Win64/msvc/debug
            }
            else {
                DESTDIR = $$BIN_PATH/$$FOLDER/Win64/msvc/release
            }
        }
    }
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
    QMAKE_CXX   = ccache $$QMAKE_CXX

    # останавливать сборку после первой ошибки
    # QMAKE_CXXFLAGS += -Wfatal-errors

    # профилирование
    # gprof ./my_prog ./gmon.out -p > result.txt
    # убрать в обычной работе
    # QMAKE_CXXFLAGS_DEBUG += -pg
    # QMAKE_LFLAGS_DEBUG += -pg

    #CONFIG += warn_off
    #QMAKE_CXXFLAGS += -Wall
    #QMAKE_CFLAGS_WARN_ON   -= -Wno-missing-braces -Wno-missing-field-initializers
    #QMAKE_CXXFLAGS_WARN_ON -= -Wno-missing-braces -Wno-missing-field-initializers
}
###############################################################################
linux {
MACHINE_ID = $$system(cat /etc/machine-id)
message("Текущий Machine ID: " $$MACHINE_ID)

contains(MACHINE_ID, "918ac8770c47e05955e6ac056282aa3a") {
    DEFINES += IS_MACHINE_HOME
    message("--- Сборка для ДОМА ---")
} else {
    DEFINES += IS_MACHINE_WORK
    message("--- Сборка для РАБОТЫ ---")
}
}

win32 {
    MACHINE_ID = $$system(powershell -Command "(Get-ItemProperty -Path 'HKLM:\SOFTWARE\Microsoft\Cryptography').MachineGuid")
    message("Текущий Machine ID: " $$MACHINE_ID)

contains(MACHINE_ID, "b87537d2-72e1-438b-8667-d41b6a9c73dd") {
    DEFINES += IS_MACHINE_HOME
    message("--- Сборка для ДОМА ---")
} else {
    DEFINES += IS_MACHINE_WORK
    message("--- Сборка для РАБОТЫ ---")
}
}
###############################################################################
greaterThan(QT_MAJOR_VERSION, 5) {
    QT += core5compat
    contains(DEFINES, LOGO_GL) {
        QT += openglwidgets
    }

linux {
    CONFIG += c++20
}
win32 {
    QMAKE_CXX   = "C:\ccache\ccache.exe $$QMAKE_CXX"

    QMAKE_CXXFLAGS += /std:c++17
    QMAKE_CXXFLAGS += /Zc:__cplusplus
    QMAKE_CXXFLAGS += /permissive-

    QMAKE_CXXFLAGS += /utf-8
}
    message(Qt6 = $$QT)
} else {
    message(Qt5 = $$QT)
}
###############################################################################
CONFIG(debug, debug|release) {
    CONFIG += console
    CONFIG -= app_bundle
    #message (console ON)
}
###############################################################################
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    DEFINES += HAVE_QT5
    #message(Qt5 = $$QT)
} else {
    DEFINES += HAVE_QT4
    #message(Qt4 = $$QT)
}
###############################################################################
# система уходит глубоко в своп с этим в Linux, не стОит
win32 {
    exists(stable.h) {
        message (PCH)
        PRECOMPILED_HEADER = stable.h
        CONFIG += precompile_header
    }
}
###############################################################################
win32:CONFIG(release, debug|release) {
    # Автоматически готовим инструмент windeployqt
    qtPrepareTool(WINDEPLOY_CMD, windeployqt)

    # Собираем команду деплоя.
    # $(DESTDIR_TARGET) — это переменная самого Makefile, она на 100% содержит путь к вашему .exe
    # Флаги экранируем от консоли Windows с помощью кавычек
    QMAKE_POST_LINK += $$escape_expand(\\n) $$WINDEPLOY_CMD --no-compiler-runtime --no-quick-import --no-translations \"$(DESTDIR_TARGET)\"
}
###############################################################################
