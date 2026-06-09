desktop.path = $$(HOME)/.local/share/applications

BIN_DIR = $$DESTDIR
isEmpty(BIN_DIR): BIN_DIR = $$OUT_PWD
ABS_BIN_DIR = $$absolute_path($$BIN_DIR, $$PWD)

# 1. Указываем точный путь к файлу иконки внутри ваших исходников
ICON_SOURCE = $$PWD/ico/computer.ico

# 2. ИСКЛЮЧАЕМ icon_deploy из INSTALLS.
# Копируем иконку к бинарнику на реальный диск силами ОС сразу после линковки бинарника.
# Это убирает конфликт путей в Makefile.
QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$ICON_SOURCE) $$quote($$ABS_BIN_DIR)

# 3. В ярлыке ссылаемся на скопированную иконку
DESKTOP_LINES = \
    "[Desktop Entry]" \
    "Type=Application" \
    "Name=$$TARGET" \
    "Comment=Template" \
    "Exec=$$ABS_BIN_DIR/$$TARGET" \
    "Icon=$$ABS_BIN_DIR/computer.ico" \
    "Terminal=false" \
    "Categories=Utility;"

write_file($$OUT_PWD/$${TARGET}.desktop, DESKTOP_LINES)
desktop.files = $$OUT_PWD/$${TARGET}.desktop

# Теперь здесь только девелоперский деплой ярлыка, никаких конфликтов с DESTDIR
INSTALLS += desktop
