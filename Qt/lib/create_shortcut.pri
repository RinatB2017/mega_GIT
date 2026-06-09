########################################################################
desktop.path = $$(HOME)/.local/share/applications

BIN_DIR = $$DESTDIR
isEmpty(BIN_DIR): BIN_DIR = $$OUT_PWD
ABS_BIN_DIR = $$absolute_path($$BIN_DIR, $$PWD)

# 1. Укажите точный путь к файлу иконки внутри ваших исходников
ICON_SOURCE = $$PWD/ico/computer.ico

# 2. Копируем этот физический файл в папку к бинарнику
icon_deploy.path  = $$ABS_BIN_DIR
icon_deploy.files = $$ICON_SOURCE

# 3. В ярлыке ссылаемся на скопированную иконку
DESKTOP_LINES = \
    "[Desktop Entry]" \
    "Type=Application" \
    "Name=$$TARGET" \
    "Comment=Template" \
    "Exec=$$ABS_BIN_DIR/$$TARGET" \
    "Icon=$$ABS_BIN_DIR/computer.ico" \
    "Terminal=false" \
    "Categories=Other;"

write_file($$OUT_PWD/$${TARGET}.desktop, DESKTOP_LINES)
desktop.files = $$OUT_PWD/$${TARGET}.desktop

INSTALLS += desktop icon_deploy
########################################################################
