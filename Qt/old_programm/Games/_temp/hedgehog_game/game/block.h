#ifndef BLOCK_H
#define BLOCK_H
#include <QWidget>

enum BlockParam {
  BlockSize = 107 // размер блока, к которому масштабируются анимации
};

class QLabel;

class Block: public QWidget {
  Q_OBJECT
public:
  explicit Block(QWidget *parent = 0);
  void animation(QString texturename, bool randomStartFrame = false);
protected:
  QMovie *m_animation;
            //!< анимация, проигрываемая блоком
  QLabel *m_label;
            //!< метка для отображения анимации
};
#endif // BLOCK_H
