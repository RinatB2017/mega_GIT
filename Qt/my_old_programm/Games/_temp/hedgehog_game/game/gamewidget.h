#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H
#include <QWidget>

//! состояние игры
enum GameWidgetState {
  Start, //!< ёж не поставлен на сцену
  BlockSelected, //!< выбран блок
  Animate, //!< проигрывается анимация
};

enum BlockLayers {
  CloudLayer, WallLayer, ActorLayer
};

enum {
  ActorType = 'a',
  CloudType = '.',
  WallType = '*'
};

class Block;
class Actor;
class QGraphicsView;
class QGraphicsScene;

class GameWidget: public QWidget {
  Q_OBJECT
public:
  GameWidget(QWidget *parent = 0);
signals:
  void gameFinished(bool);
protected slots:
  void onBlockClicked();
  void onMoveFinished();
              //!< завершено перемещение ежа
public slots:
  void load(int n, int m, QString data);
protected:
  virtual void resizeEvent(QResizeEvent *event);
  virtual void blockClickedHandler(Block* block);
protected:
  virtual void itemAdd(int i, int j, char type);
  bool isBorder(int i, int j);
              //!< проверяет отсутсвие проходимости клетки

  const int Delay;
              //!< время перемещения автора
  const QPoint ActorShift;
              //!< смещение актора относительно выбранной похиции (для красоты)

  GameWidgetState m_state;
  QGraphicsScene *m_scene;
  QGraphicsView *m_view;
  Actor *m_actor;
  std::vector<std::vector<Block*> > m_items;
};
#endif // GAMEWIDGET_H
