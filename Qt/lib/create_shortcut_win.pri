# =============================================================================
# НАСТРОЙКИ ЯРЛЫКА (Задаются в .pro файле или прямо здесь)
# =============================================================================
isEmpty(TARGET): TARGET = $$basename(_PRO_FILE_PWD_)

isEmpty(KDE_SHORTCUT_NAME):        KDE_SHORTCUT_NAME = $$TARGET
isEmpty(KDE_SHORTCUT_GENERIC):     KDE_SHORTCUT_GENERIC = "Application"
isEmpty(KDE_SHORTCUT_COMMENT):     KDE_SHORTCUT_COMMENT = "Built with Qt6"

# Корректная обработка путей для Exec и Icon в зависимости от ОС
linux {
    isEmpty(KDE_SHORTCUT_EXEC):    KDE_SHORTCUT_EXEC = $$clean_path($$DESTDIR/$$TARGET)
    isEmpty(KDE_SHORTCUT_ICON):    KDE_SHORTCUT_ICON = $$_PRO_FILE_PWD_/ico/computer.ico
}
win32 {
    # В Windows исполняемый файл имеет расширение .exe, а слэши должны быть обратными
    isEmpty(KDE_SHORTCUT_EXEC):    KDE_SHORTCUT_EXEC = $$clean_path($$DESTDIR/$$TARGET\.exe)
    isEmpty(KDE_SHORTCUT_ICON):    KDE_SHORTCUT_ICON = $$clean_path($$_PRO_FILE_PWD_/ico/computer.ico)

    # Меняем прямые слэши на обратные (специфика Windows/CMD)
    KDE_SHORTCUT_EXEC = $$replace(KDE_SHORTCUT_EXEC, /, \\)
    KDE_SHORTCUT_ICON = $$replace(KDE_SHORTCUT_ICON, /, \\)
}

isEmpty(KDE_SHORTCUT_CATEGORIES):  KDE_SHORTCUT_CATEGORIES = "Other;"
isEmpty(KDE_SHORTCUT_TERMINAL):    KDE_SHORTCUT_TERMINAL = "false"
isEmpty(KDE_SHORTCUT_FILENAME):    KDE_SHORTCUT_FILENAME = "org.qtproject.$$TARGET"

# =============================================================================
# АВТОМАТИЧЕСКАЯ ГЕНЕРАЦИЯ И СБОРКА ДЛЯ LINUX
# =============================================================================
linux {
    KDE_MENU_DIR = $$system("echo ~/.local/share/applications")
    KDE_SHORTCUT_DEST = $$KDE_MENU_DIR/$$KDE_SHORTCUT_FILENAME\.desktop

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

    QMAKE_POST_LINK += $$KDE_SHORTCUT_COMMANDS
}

# =============================================================================
# АВТОМАТИЧЕСКАЯ ГЕНЕРАЦИЯ И СБОРКА ДЛЯ WINDOWS (XP / 11)
# =============================================================================
win32 {
    # Скрипт генерации ярлыка через встроенный VBScript (работает везде от XP до Win11)
    # Используем ^ для экранирования перевода строк в cmd.exe
    WIN_SHORTCUT_COMMANDS = @echo Generating Windows Start Menu shortcut... \
        && echo Set oWS = CreateObject("WScript.Shell") > create_lnk.vbs \
        && echo sLinkFile = oWS.ExpandEnvironmentStrings("%APPDATA%\Microsoft\Windows\Start Menu\Programs\$$KDE_SHORTCUT_NAME\.lnk") >> create_lnk.vbs \
        && echo Set oLink = oWS.CreateShortcut(sLinkFile) >> create_lnk.vbs \
        && echo oLink.TargetPath = "$$KDE_SHORTCUT_EXEC" >> create_lnk.vbs \
        && echo oLink.Description = "$$KDE_SHORTCUT_COMMENT" >> create_lnk.vbs \
        && echo oLink.IconLocation = "$$KDE_SHORTCUT_ICON" >> create_lnk.vbs \
        && echo oLink.Save >> create_lnk.vbs \
        && cscript //nologo create_lnk.vbs \
        && del create_lnk.vbs \
        && echo Windows shortcut successfully created!

    # Переменная внедряет команды в конец сборки для Windows (в nmake или mingw32-make)
    QMAKE_POST_LINK += $$WIN_SHORTCUT_COMMANDS
}
