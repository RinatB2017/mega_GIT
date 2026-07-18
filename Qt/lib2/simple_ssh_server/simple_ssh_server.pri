#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += simple_ssh_server.hpp
SOURCES += simple_ssh_server.cpp

# Настройки для Windows-сборки
win32 {
    # Указываем компилятору, где искать заголовочные файлы libssh
    INCLUDEPATH += $$PWD/3rdparty/include

    # Указываем линкеру путь к библиотеке
    # (вместо -lssh подставляем полный путь к dll или lib файлу)
    LIBS += -L$$PWD/3rdparty/ -lssh
}

# Ваши текущие настройки для Linux (оставляем как есть)
unix:!macx {
    LIBS += -lssh
    LIBS += -lutil
}
