#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Template

FOLDER  = _Templates

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    music/music.qrc \
    images/images.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)

include ($$PWD/src/template_mainbox/template_mainbox.pri)

VPATH = $$INCLUDEPATH

########################################################################
desktop.path = $$(HOME)/.local/share/applications

BIN_DIR = $$DESTDIR
isEmpty(BIN_DIR): BIN_DIR = $$OUT_PWD
ABS_BIN_DIR = $$absolute_path($$BIN_DIR, $$PWD)

# 1. Укажите точный путь к файлу иконки внутри ваших исходников
ICON_SOURCE = $$PWD/src/mainwindow/computer.png

# 2. Копируем этот физический файл в папку к бинарнику
icon_deploy.path  = $$ABS_BIN_DIR
icon_deploy.files = $$ICON_SOURCE

# 3. В ярлыке ссылаемся на скопированную иконку
DESKTOP_LINES = \
    "[Desktop Entry]" \
    "Type=Application" \
    "Name=$$TARGET" \
    "Exec=$$ABS_BIN_DIR/$$TARGET" \
    "Icon=$$ABS_BIN_DIR/computer.png" \
    "Terminal=false" \
    "Categories=X-Personal;"

write_file($$OUT_PWD/$${TARGET}.desktop, DESKTOP_LINES)
desktop.files = $$OUT_PWD/$${TARGET}.desktop

INSTALLS += desktop icon_deploy
########################################################################

