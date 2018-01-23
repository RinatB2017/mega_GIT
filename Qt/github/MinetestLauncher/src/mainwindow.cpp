#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->createBtn,  SIGNAL(clicked()),  this,   SLOT(newProfile()));
    connect(ui->deleteBtn,  SIGNAL(clicked()),  this,   SLOT(deleteSelected()));
    connect(ui->playBtn,    SIGNAL(clicked()),  this,   SLOT(playSelected()));

    QFont font;
    font.setPointSize(32);
    ui->greetingText->setFont(font);

    dataManager->setManager(profileManager);
    dataManager->loadProfiles();
    profileManager->refreshList(ui->comboBox);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newProfile()
{
    CreateDialog::newProfile(profileManager);
    profileManager->refreshList(ui->comboBox);
    dataManager->saveProfiles();
}

void MainWindow::deleteSelected()
{
    profileManager->deleteByName(ui->comboBox->currentText());
    profileManager->refreshList(ui->comboBox);
    dataManager->saveProfiles();
}

void MainWindow::playSelected()
{
    profileManager->getByName(ui->comboBox->currentText()).play(ui->tabWidget);
}
