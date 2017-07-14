#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD
INCLUDEPATH = $$DEPENDPATH

TRANSLATIONS += programm/*.ts
OTHER_FILES  += programm/*.ts

RESOURCES   +=	programm/programm.qrc

isEmpty(QMAKE_LRELEASE) {
    win32|os2:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
    unix {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease }
    } else {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease }
    }
}

updateqm.input = TRANSLATIONS
updateqm.output = locale/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$QMAKE_LRELEASE -silent ${QMAKE_FILE_IN} -qm locale/${QMAKE_FILE_BASE}.qm
updateqm.CONFIG += no_link target_predeps no_clean
QMAKE_EXTRA_COMPILERS += updateqm

#QMAKE_EXTRA_TARGETS += updateqm
#POST_TARGETDEPS += updateqm

#message ($$QMAKE_LRELEASE)
