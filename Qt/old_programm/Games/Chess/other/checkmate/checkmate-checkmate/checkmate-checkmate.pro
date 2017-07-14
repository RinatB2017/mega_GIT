
TEMPLATE = app
TARGET = chess

DEPENDPATH += . src
INCLUDEPATH += . src

QT  += svg xml

# Input
HEADERS += \
    src/abstractengine.h \
    src/advancegame.h \
    src/board.h \
    src/checkmate.h \
    src/chessutilities.h \
    src/commandedit.h \
    src/commandhighlighter.h \
    src/gameconsole.h \
    src/gameinstance.h \
    src/gnuchessengine.h \
    src/movelist.h \
    src/movement.h \
    src/movetableitem.h \
    src/movetablemodel.h \
    src/movetableview.h \
    src/piece.h \
    src/pieceinstance.h \
    src/promotiondialog.h \
    src/themeprovider.h

SOURCES += \
    src/abstractengine.cpp \
    src/advancegame.cpp \
    src/board.cpp \
    src/checkmate.cpp \
    src/chessutilities.cpp \
    src/commandedit.cpp \
    src/commandhighlighter.cpp \
    src/gameconsole.cpp \
    src/gameinstance.cpp \
    src/gnuchessengine.cpp \
    src/main.cpp \
    src/movelist.cpp \
    src/movement.cpp \
    src/movetableitem.cpp \
    src/movetablemodel.cpp \
    src/movetableview.cpp \
    src/piece.cpp \
    src/pieceinstance.cpp \
    src/promotiondialog.cpp \
    src/themeprovider.cpp

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    message(Qt5 = $$QT)
}
