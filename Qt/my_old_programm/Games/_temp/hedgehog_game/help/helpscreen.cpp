#include "helpscreen.h"
#include "ui_helpscreen.h"

HelpScreen::HelpScreen(QWidget *parent): QWidget(parent),
    ui(new Ui::HelpScreen) {
    ui->setupUi(this);

    connect(ui->demo, SIGNAL(gameFinished(bool)), SLOT(load()));
    connect(ui->rulesButton, SIGNAL(clicked()), SLOT(on_RulesClicked()));
    connect(ui->demoButton, SIGNAL(clicked()), SLOT(on_DemoClicked()));
    connect(ui->menuButton, SIGNAL(clicked()), SLOT(on_MenuClicked()));

    state(HelpScreenState::Rules);
}

HelpScreen::~HelpScreen() {
    delete ui;
}

void HelpScreen::on_DemoClicked() {
    state(HelpScreenState::Demo);
}

void HelpScreen::on_RulesClicked() {
    state(HelpScreenState::Rules);
}

void HelpScreen::on_MenuClicked() {
    emit gameClicked();
}

void HelpScreen::state(HelpScreenState state) {
    switch (state) {
    case HelpScreenState::Rules:
        ui->demo->hide();
        ui->rules->show();
        ui->rulesButton->setEnabled(false);
        ui->demoButton->setEnabled(true);
        break;
    case HelpScreenState::Demo:
        ui->demo->show();
        ui->rules->hide();
        ui->rulesButton->setEnabled(true);
        ui->demoButton->setEnabled(false);
        break;
    default:
        break;
    }
}

void HelpScreen::load() {
    QList<QPoint> solution;

    solution << QPoint(3, 2) << QPoint(3, 0) << QPoint(4, 0)
             << QPoint(4, 4) << QPoint(0, 4) << QPoint(0, 0)
             << QPoint(2, 0) << QPoint(2, 3) << QPoint(1, 3)
             << QPoint(1, 2);

    ui->demo->load(5, 5,
                   "....."
                   ".*..."
                   "....."
                   "...*."
                   ".....");

    ui->demo->loadSolution(solution);
}
