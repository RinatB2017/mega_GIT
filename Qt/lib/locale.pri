#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD
INCLUDEPATH = $$DEPENDPATH

# почему то винде не работает *.
#linux {
#    TRANSLATIONS    += locale/*.ts
#}

#win32 {
    TRANSLATIONS    += locale/lang_ru.ts
    TRANSLATIONS    += locale/lang_it.ts
#}

isEmpty(QMAKE_LRELEASE) {
    win32|os2:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
    linux {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease }
    } else {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease }
    }
}

# начиная с Qt 5.3 для корректного перевода надо загружать ВСЕ нужные файлы, т.е
# qtbase_ru.qm
# qtmultimedia_ru.qm
# qtquick1_ru.qm
# qtscript_ru.qm
# qtxmlpatterns_ru.qm
# лучше грузить из ресурсов, т.к. на целевой машине, скорее всего, Qt не установлен

RESOURCES += $$PWD/system/system.qrc

updateqm.input = TRANSLATIONS
updateqm.output = locale/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$QMAKE_LRELEASE -silent ${QMAKE_FILE_IN} -qm locale/${QMAKE_FILE_BASE}.qm
updateqm.CONFIG += no_link target_predeps no_clean
QMAKE_EXTRA_COMPILERS += updateqm
