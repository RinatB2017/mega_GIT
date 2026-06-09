desktop.path = $$(HOME)/.local/share/applications

BIN_DIR = $$DESTDIR
isEmpty(BIN_DIR): BIN_DIR = $$OUT_PWD
ABS_BIN_DIR = $$absolute_path($$BIN_DIR, $$PWD)

# 1. Путь к исходной иконке
ICON_SOURCE = $$PWD/ico/computer.ico

# 2. Копирование иконки к бинарнику
icon_deploy.path  = $$ABS_BIN_DIR
icon_deploy.files = $$ICON_SOURCE

# 3. Безопасное создание .desktop файла через сборочные цели (Make)
DESKTOP_FILE = $$OUT_PWD/$${TARGET}.desktop

# Описываем команды для создания файла силами самой ОС при сборке
desktop_gen.target = $$DESKTOP_FILE
desktop_gen.commands = \
    @echo "[Desktop Entry]" > $$DESKTOP_FILE && \
    @echo "Type=Application" >> $$DESKTOP_FILE && \
    @echo "Name=$$TARGET" >> $$DESKTOP_FILE && \
    @echo "Comment=Template" >> $$DESKTOP_FILE && \
    @echo "Exec=$$ABS_BIN_DIR/$$TARGET" >> $$DESKTOP_FILE && \
    @echo "Icon=$$ABS_BIN_DIR/computer.ico" >> $$DESKTOP_FILE && \
    @echo "Terminal=false" >> $$DESKTOP_FILE && \
    @echo "Categories=Utility;" >> $$DESKTOP_FILE

# Добавляем генератор в процесс сборки
QMAKE_EXTRA_TARGETS += desktop_gen
PRE_TARGETDEPS += $$DESKTOP_FILE

# 4. Указываем файл для деплоя
desktop.files = $$DESKTOP_FILE

INSTALLS += desktop icon_deploy
