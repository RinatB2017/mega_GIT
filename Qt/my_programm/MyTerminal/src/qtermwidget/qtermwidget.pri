
DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += HAVE_POSIX_OPENPT

HEADERS += \
    BlockArray.h \
    Character.h \
    CharacterColor.h \
    ColorScheme.h \
    ColorTables.h \
    DefaultTranslatorText.h \
    Emulation.h \
    ExtendedDefaultTranslator.h \
    Filter.h \
    History.h \
    HistorySearch.h \
    KeyboardTranslator.h \
    konsole_wcwidth.h \
    kprocess.h \
    kpty.h \
    kpty_p.h \
    kptydevice.h \
    kptyprocess.h \
    LineFont.h \
    Pty.h \
    qtermwidget.h \
    Screen.h \
    ScreenWindow.h \
    SearchBar.h \
    Session.h \
    ShellCommand.h \
    TerminalCharacterDecoder.h \
    TerminalDisplay.h \
    tools.h \
    Vt102Emulation.h \
    qtermwidget_export.h \
    qtermwidget_version.h

SOURCES += \
    BlockArray.cpp \
    ColorScheme.cpp \
    Emulation.cpp \
    Filter.cpp \
    History.cpp \
    HistorySearch.cpp \
    KeyboardTranslator.cpp \
    konsole_wcwidth.cpp \
    kprocess.cpp \
    kpty.cpp \
    kptydevice.cpp \
    kptyprocess.cpp \
    Pty.cpp \
    qtermwidget.cpp \
    Screen.cpp \
    ScreenWindow.cpp \
    SearchBar.cpp \
    Session.cpp \
    ShellCommand.cpp \
    TerminalCharacterDecoder.cpp \
    TerminalDisplay.cpp \
    tools.cpp \
    Vt102Emulation.cpp

FORMS += SearchBar.ui
           
TRANSLATIONS += \
    translations/qtermwidget.ts \
    translations/qtermwidget_arn.ts \
    translations/qtermwidget_ast.ts \
    translations/qtermwidget_ca.ts \
    translations/qtermwidget_cs.ts \
    translations/qtermwidget_cy.ts \
    translations/qtermwidget_da.ts \
    translations/qtermwidget_de.ts \
    translations/qtermwidget_el.ts \
    translations/qtermwidget_es.ts \
    translations/qtermwidget_fr.ts \
    translations/qtermwidget_gl.ts \
    translations/qtermwidget_he.ts \
    translations/qtermwidget_hu.ts \
    translations/qtermwidget_ja.ts \
    translations/qtermwidget_lt.ts \
    translations/qtermwidget_nb_NO.ts \
    translations/qtermwidget_pl.ts \
    translations/qtermwidget_pt.ts \
    translations/qtermwidget_pt_BR.ts \
    translations/qtermwidget_tr.ts \
    translations/qtermwidget_zh_CN.ts \
    translations/qtermwidget_zh_TW.ts
