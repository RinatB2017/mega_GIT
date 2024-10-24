
DEPENDPATH  += \
    $$PWD
#    $$PWD/ScintillaEditBase \
#    $$PWD/include \
#    $$PWD/src \
#    $$PWD/lexlib
INCLUDEPATH = $$DEPENDPATH

SOURCES += \
    ScintillaEdit.cpp \
    ScintillaDocument.cpp \
    ScintillaEditBase/PlatQt.cpp \
    ScintillaEditBase/ScintillaQt.cpp \
    ScintillaEditBase/ScintillaEditBase.cpp \
    src/XPM.cxx \
    src/ViewStyle.cxx \
    src/UniConversion.cxx \
    src/Style.cxx \
    src/Selection.cxx \
    src/ScintillaBase.cxx \
    src/RunStyles.cxx \
    src/RESearch.cxx \
    src/PositionCache.cxx \
    src/PerLine.cxx \
    src/MarginView.cxx \
    src/LineMarker.cxx \
    src/KeyMap.cxx \
    src/Indicator.cxx \
    src/ExternalLexer.cxx \
    src/EditView.cxx \
    src/Editor.cxx \
    src/EditModel.cxx \
    src/Document.cxx \
    src/Decoration.cxx \
    src/DBCS.cxx \
    src/ContractionState.cxx \
    src/CharClassify.cxx \
    src/CellBuffer.cxx \
    src/Catalogue.cxx \
    src/CaseFolder.cxx \
    src/CaseConvert.cxx \
    src/CallTip.cxx \
    src/AutoComplete.cxx \
    lexlib/WordList.cxx \
    lexlib/StyleContext.cxx \
    lexlib/PropSetSimple.cxx \
    lexlib/LexerSimple.cxx \
    lexlib/LexerNoExceptions.cxx \
    lexlib/LexerModule.cxx \
    lexlib/LexerBase.cxx \
    lexlib/DefaultLexer.cxx \
    lexlib/CharacterSet.cxx \
    lexlib/CharacterCategory.cxx \
    lexlib/Accessor.cxx \
    $$files(lexers/*.cxx, false)

HEADERS  += \
    ScintillaEdit.h \
    ScintillaDocument.h \
    ScintillaEditBase/ScintillaEditBase.h \
    ScintillaEditBase/ScintillaQt.h
