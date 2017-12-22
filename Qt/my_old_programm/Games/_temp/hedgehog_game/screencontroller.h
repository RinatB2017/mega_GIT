#ifndef SCREENCONTROLLER_H
#define SCREENCONTROLLER_H

#include <QWidget>

class GameScreen;
class HelpScreen;
class WinScreen;

class ScreenController: public QWidget {
  Q_OBJECT
public:
  explicit ScreenController(QWidget *parent = 0);
private slots:
  void on_2game();
    //!< переход на экран игры
  void on_2help();
    //!< переход на экран помощи
protected slots:
  void on_gameFinished(bool gameResult);
protected:
  GameScreen *m_pGame;
  HelpScreen *m_pHelp;
  WinScreen *m_pWin;
};

#endif // SCREENCONTROLLER_H
