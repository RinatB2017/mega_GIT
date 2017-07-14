#ifndef GAMEHELPWIDGET_H
#define GAMEHELPWIDGET_H
#include "gamewidget.h"

enum {
  CloudHelpType = 'h',
};

class GameHelpWidget: public GameWidget {
  Q_OBJECT
public:
  explicit GameHelpWidget(QWidget *parent = 0);
  void loadSolution(QList<QPoint> solution);
protected:
  virtual void blockClickedHandler(Block *block);
  virtual void itemAdd(int i, int j, char type);
              //!< добавляет айтем на игровое поле
protected:
  QList<QPoint> m_solution;

  int m_i, m_j;
    //!< текущая позиция подсказки

};

#endif // GAMEHELPWIDGET_H
