#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <QWidget>

namespace Ui {
  class GameScreen;
}

class GameScreen : public QWidget {
  Q_OBJECT
public:
  explicit GameScreen(QWidget *parent = 0);
  virtual ~GameScreen();
public slots:
  void reload();
  void load(int n, int m, QString data);
private slots:
  void on_helpClicked();
  void on_finished(bool gameResult);
signals:
  void helpClicked();
  void finished(bool gameResult);
protected:
  Ui::GameScreen *ui;
  QString m_levelData;
  int m_n, m_m;
};

#endif // GAMESCREEN_H
