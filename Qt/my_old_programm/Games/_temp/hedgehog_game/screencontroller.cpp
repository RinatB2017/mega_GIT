#include "screencontroller.h"

#include "game/gamescreen.h"
#include "help/helpscreen.h"
#include "win/winscreen.h"

#include <QGridLayout>

ScreenController::ScreenController(QWidget *parent): QWidget(parent),
    m_pGame(new GameScreen(this)),
    m_pHelp(new HelpScreen(this)),
    m_pWin(new WinScreen(this)) {

    QGridLayout *layout(new QGridLayout(this));

    layout->addWidget(m_pGame, 0, 0, 1, 1);
    layout->addWidget(m_pHelp, 0, 0, 1, 1);
    layout->addWidget(m_pWin, 0, 0, 1, 1);

    connect(m_pHelp, SIGNAL(gameClicked()), SLOT(on_2game()));
    connect(m_pGame, SIGNAL(helpClicked()), SLOT(on_2help()));
    connect(m_pWin, SIGNAL(replayClicked()), SLOT(on_2game()));
    connect(m_pGame, SIGNAL(finished(bool)), SLOT(on_gameFinished(bool)));

    on_2game();

    m_pGame->load(6, 6,
                  "......"
                  "......"
                  ".*...."
                  "......"
                  "..*..."
                  "..*...");
    m_pHelp->load();


    setStyleSheet(
                "* { background-color: rgba(176, 196, 222, 255); }"
                "QPushButton { "
                "  background-color: rgba(255, 153, 102, 200); "
                "  border-style: outset;"
                "  border-width: 2px;"
                "  border-radius: 10px;"
                "  border-color: beige;"
                "  font: bold 14px;"
                "  width: 3em;"
                "  padding: 6px;"
                "}"
                "QPushButton:hover {"
                "background-color: rgba(255, 102, 0, 200);"
                "}"
                "QPushButton:pressed {"
                "background-color: rgba(255, 0, 0, 200);"
                "}"
                "QPushButton:disabled {"
                "background-color: rgba(204, 153, 102, 200);"
                "}"
                "QTextEdit {"
                "background-color: rgba(102, 204, 102, 200);"
                "  border-style: outset;"
                "  border-width: 0px;"
                "  border-radius: 10px;"
                "  border-color: black;"
                "  width: 3em;"
                "  padding: 6px;"
                "}"
                );
}

void ScreenController::on_2game() {
    m_pGame->show();
    m_pHelp->hide();
    m_pWin->hide();
}

void ScreenController::on_2help() {
    m_pGame->hide();
    m_pHelp->show();
}

void ScreenController::on_gameFinished(bool gameResult) {
    m_pGame->reload();
    if (gameResult) {
        m_pGame->hide();
        m_pWin->show();
    }
}
