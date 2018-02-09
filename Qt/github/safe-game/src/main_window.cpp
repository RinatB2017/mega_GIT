#include "main_window.h"

#include <QAction>
#include <QComboBox>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QTimer>
#include <QVBoxLayout>

#include "hall_of_fame_dialog.h"
#include "history.h"
#include "safe_widget.h"
#include "utils.h"

safe::MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Safe Game"));

    updateTime_ = new QTimer(this);
    updateTime_->setInterval(1000); // one second
    connect(updateTime_, &QTimer::timeout, this, &MainWindow::onUpdateTime);

    time_ = new QLabel();

    auto info = new QHBoxLayout();
    info->addStretch();
    info->addWidget(time_);
    info->addStretch();

    placeForSafe_ = new QHBoxLayout();

    size_ = new QComboBox();
    for (auto i = MinSize; i <= MaxSize; ++i)
    {
        size_->addItem(QString::number(i), i);
    }

    auto restart = new QPushButton(tr("Restart"));
    connect(restart, &QPushButton::clicked, this, &MainWindow::onStartNewGame);

    auto controls = new QHBoxLayout();
    controls->addStretch();
    controls->addWidget(size_);
    controls->addWidget(restart);
    controls->addStretch();

    auto layout = new QVBoxLayout();
    layout->addLayout(info);
    layout->addStretch();
    layout->addLayout(placeForSafe_);
    layout->addStretch();
    layout->addLayout(controls);

    auto frame = new QFrame();
    frame->setLayout(layout);

    setCentralWidget(frame);

    history_ = new History(this);

    createMenu();

    onStartNewGame();
}

safe::MainWindow::~MainWindow()
{
}

void safe::MainWindow::onPlayerWon()
{
    updateTime_->stop();
    HallOfFameDialog::showHall(secondsFromStart_);
}

void safe::MainWindow::onUpdateTime()
{
    ++secondsFromStart_;
    showTime();
}

void safe::MainWindow::onStartNewGame()
{
    auto old = placeForSafe_->takeAt(0);
    if (old != nullptr && !old->isEmpty())
    {
        delete old->widget();
    }

    secondsFromStart_ = 0;
    showTime();
    updateTime_->start();

    const auto size = size_->currentData().toInt();

    auto safe = new SafeWidget(history_, size);
    connect(safe, &SafeWidget::playerWon, this, &MainWindow::onPlayerWon);

    placeForSafe_->addWidget(safe);

    centralWidget()->adjustSize();
    adjustSize();
}

void safe::MainWindow::showTime()
{
    time_->setText(secondsToString(secondsFromStart_));
}

void safe::MainWindow::createMenu()
{
    auto exit = new QAction(tr("E&xit"), this);
    connect(exit, &QAction::triggered, this, &QMainWindow::close);

    auto file = new QMenu(tr("&File"));
    file->addAction(exit);

    auto undo = history_->createUndoAction(this, tr("&Undo"));
    undo->setShortcut(QKeySequence::Undo);

    auto redo = history_->createRedoAction(this, tr("&Redo"));
    redo->setShortcut(QKeySequence::Redo);

    auto edit = new QMenu(tr("&Edit"));
    edit->addAction(undo);
    edit->addAction(redo);

    auto hallOfFame = new QAction(tr("Hall of fame"), this);
    connect(hallOfFame, &QAction::triggered, this, &MainWindow::onShowHallOfFame);

    auto about = new QAction(tr("About"), this);
    connect(about, &QAction::triggered, this, &MainWindow::onShowAbout);

    auto info = new QMenu(tr("Info"));
    info->addAction(hallOfFame);
    info->addSeparator();
    info->addAction(about);

    auto menu = new QMenuBar();
    menu->addMenu(file);
    menu->addMenu(edit);
    menu->addMenu(info);

    setMenuBar(menu);
}

void safe::MainWindow::onShowHallOfFame()
{
    HallOfFameDialog::showHall();
}

void safe::MainWindow::onShowAbout()
{
    QMessageBox::about(nullptr, tr("Safe game"), tr("Set all the switches to the horizontal position to win."));
}
