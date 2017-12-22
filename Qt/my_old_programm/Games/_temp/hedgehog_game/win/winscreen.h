#ifndef WINSCREEN_H
#define WINSCREEN_H

#include <QWidget>

namespace Ui {
  class WinScreen;
}

class WinScreen : public QWidget {
  Q_OBJECT
public:
  explicit WinScreen(QWidget *parent = 0);
  ~WinScreen();
private slots:
  void on_replayClicked();
signals:
  void replayClicked();
private:
  Ui::WinScreen *ui;
};

#endif // WINSCREEN_H
