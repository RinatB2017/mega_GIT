#ifndef HELPSCREEN_H
#define HELPSCREEN_H

#include <QWidget>

namespace Ui {
  class HelpScreen;
}

enum HelpScreenState {
  Rules, Demo
};

class HelpScreen : public QWidget {
  Q_OBJECT

public:
  explicit HelpScreen(QWidget *parent = 0);
  ~HelpScreen();

signals:
  void gameClicked();
    //!< запрос возврата на экран игры

public slots:
  void load();
  void state(HelpScreenState state);
private slots:
  void on_DemoClicked();
  void on_RulesClicked();
  void on_MenuClicked();

private:
  Ui::HelpScreen *ui;

  HelpScreenState m_state;
};

#endif // HELPSCREEN_H
