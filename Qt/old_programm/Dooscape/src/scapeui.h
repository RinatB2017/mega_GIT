#ifndef SCAPEUI_H
#define SCAPEUI_H

#include <QtWidgets>

class ScapeUI : public QObject
{
    Q_OBJECT
    public:
        explicit ScapeUI(QObject *parent = 0);

    signals:

    public slots:

};

class HideBlock : public QFrame
{
    Q_OBJECT
    public:
        explicit HideBlock(QString title = tr("Block"), QWidget *parent = 0);

    signals:

    public slots:
        void setBlockLayout(QLayout *layout);

    private slots:
        void showBlock();

    private:
        QFrame *block;
        QToolButton *btnTitle;
        int blockHeight;

};

#endif // SCAPEUI_H
