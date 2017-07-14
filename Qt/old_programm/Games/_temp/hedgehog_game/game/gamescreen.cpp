#include "gamescreen.h"
#include "ui_gamescreen.h"

GameScreen::GameScreen(QWidget *parent): QWidget(parent),
    ui(new Ui::GameScreen) {
    ui->setupUi(this);

    connect(ui->reload, SIGNAL(clicked()), SLOT(reload()));
    connect(ui->helpButton, SIGNAL(clicked()), SLOT(on_helpClicked()));

    connect(ui->gamewidget, SIGNAL(gameFinished(bool)), SLOT(on_finished(bool)));
}

GameScreen::~GameScreen() {
    delete ui;
}

void GameScreen::load(int n, int m, QString data) {
    m_n = n, m_m = m, m_levelData = data;
    reload();
}

void GameScreen::reload() {
    ui->gamewidget->load(m_n, m_m, m_levelData);
}

void GameScreen::on_helpClicked() {
    emit helpClicked();
}

void GameScreen::on_finished(bool gameResult) {
    emit finished(gameResult);
}
