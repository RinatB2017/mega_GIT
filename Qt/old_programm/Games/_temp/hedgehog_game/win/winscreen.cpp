#include "winscreen.h"
#include "ui_winscreen.h"

WinScreen::WinScreen(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::WinScreen) {
  ui->setupUi(this);

  connect(ui->replay, SIGNAL(clicked()), SLOT(on_replayClicked()));
}

WinScreen::~WinScreen() {
  delete ui;
}

void WinScreen::on_replayClicked() {
  emit replayClicked();
}
