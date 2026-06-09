# =============================================================================
# НАСТРОЙКИ ЯРЛЫКА ДЛЯ KDE (Задаются в .pro файле или прямо здесь)
# =============================================================================
isEmpty(TARGET): TARGET = $$basename(_PRO_FILE_PWD_) # Защита от пустой переменной TARGET

isEmpty(KDE_SHORTCUT_NAME):        KDE_SHORTCUT_NAME = $$TARGET
isEmpty(KDE_SHORTCUT_GENERIC):     KDE_SHORTCUT_GENERIC = "Application"
isEmpty(KDE_SHORTCUT_COMMENT):     KDE_SHORTCUT_COMMENT = "Built with Qt6"
isEmpty(KDE_SHORTCUT_EXEC):        KDE_SHORTCUT_EXEC = $$clean_path($$DESTDIR/$$TARGET)
isEmpty(KDE_SHORTCUT_ICON):        KDE_SHORTCUT_ICON = $$_PRO_FILE_PWD_/ico/computer.ico
isEmpty(KDE_SHORTCUT_CATEGORIES):  KDE_SHORTCUT_CATEGORIES = "Other;"
isEmpty(KDE_SHORTCUT_TERMINAL):    KDE_SHORTCUT_TERMINAL = "false"
isEmpty(KDE_SHORTCUT_FILENAME):    KDE_SHORTCUT_FILENAME = "org.qtproject.$$TARGET\.desktop"

# =============================================================================
# АВТОМАТИЧЕСКАЯ ГЕНЕРАЦИЯ И СБОРКА (МАГИЯ QMAKE)
# =============================================================================

linux {
    # Определяем путь к меню приложений пользователя (~/.local/share/applications)
    KDE_MENU_DIR = $$system("echo ~/.local/share/applications")
    KDE_SHORTCUT_DEST = $$KDE_MENU_DIR/$$KDE_SHORTCUT_FILENAME

    # Формируем цепочку bash-команд, разделенных через '&&'
    KDE_SHORTCUT_COMMANDS =  @echo "Generating KDE shortcut..." \
                             "&& mkdir -p $$KDE_MENU_DIR" \
                             "&& echo '[Desktop Entry]' > $$KDE_SHORTCUT_DEST" \
                             "&& echo 'Version=1.0' >> $$KDE_SHORTCUT_DEST" \
                             "&& echo 'Type=Application' >> $$KDE_SHORTCUT_DEST" \
                             "&& echo 'Name=$$KDE_SHORTCUT_NAME' >> $$KDE_SHORTCUT_DEST" \
                             "&& echo 'GenericName=$$KDE_SHORTCUT_GENERIC' >> $$KDE_SHORTCUT_DEST" \
                             "&& echo 'Comment=$$KDE_SHORTCUT_COMMENT' >> $$KDE_SHORTCUT_DEST" \
                             "&& echo 'Exec=$$KDE_SHORTCUT_EXEC' >> $$KDE_SHORTCUT_DEST" \
                             "&& echo 'Icon=$$KDE_SHORTCUT_ICON' >> $$KDE_SHORTCUT_DEST" \
                             "&& echo 'Terminal=$$KDE_SHORTCUT_TERMINAL' >> $$KDE_SHORTCUT_DEST" \
                             "&& echo 'Categories=$$KDE_SHORTCUT_CATEGORIES' >> $$KDE_SHORTCUT_DEST" \
                             "&& chmod +x $$KDE_SHORTCUT_DEST" \
                             "&& echo 'Updating KDE SycoCa cache...'" \
                             "&& (kbuildsycoca6 --noincremental > /dev/null 2>&1 || kbuildsycoca5 --noincremental > /dev/null 2>&1 || true)" \
                             "&& echo 'KDE shortcut successfully created!'"

    # Используем QMAKE_POST_LINK вместо создания отдельного extra target.
    # Эта переменная внедряет команды прямо в конец сборки основной цели Makefile.
    QMAKE_POST_LINK += $$KDE_SHORTCUT_COMMANDS

    # удаление
    QMAKE_CLEAN += $$KDE_SHORTCUT_DEST
}
