//--------------------------------------------------------------------------------
#include <QGridLayout>
#include <QPushButton>
#include <QVariant>
#include <QLabel>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "cell_io.hpp"
//--------------------------------------------------------------------------------
Cell_IO::Cell_IO(QWidget *parent) :
    QGridLayout(parent),
    parent(parent)
{
    setSpacing(0);
    setMargin(0);
}
//--------------------------------------------------------------------------------
void Cell_IO::create_label(unsigned int x,
                           unsigned int y,
                           const QString &caption)
{
    if(x > (MAX_X - 1))
    {
        qDebug() << QString("create_label X: %1 > %2")
                    .arg(x)
                    .arg(MAX_X - 1);
        return;
    }
    if(y > (MAX_Y - 1))
    {
        qDebug() << QString("create_label Y: %1 > %2")
                    .arg(y)
                    .arg(MAX_Y - 1);
        return;
    }
    QLabel *label = new QLabel(caption);
    addWidget(label, y, x);
}
//--------------------------------------------------------------------------------
void Cell_IO::create_button(unsigned int x,
                            unsigned int y,
                            const QString &caption)
{
    if(x > (MAX_X - 1))
    {
        qDebug() << QString("create_button X: %1 > %2")
                    .arg(x)
                    .arg(MAX_X - 1);
        return;
    }
    if(y > (MAX_Y - 1))
    {
        qDebug() << QString("create_button Y: %1 > %2")
                    .arg(y)
                    .arg(MAX_Y - 1);
        return;
    }
    buttons[x][y] = new QPushButton(caption);
    buttons[x][y]->setCheckable(true);
    buttons[x][y]->setProperty("X", x);
    buttons[x][y]->setProperty("Y", y);
    addWidget(buttons[x][y], y, x);
    connect(buttons[x][y], SIGNAL(clicked(bool)), this, SLOT(click(bool)));
}
//--------------------------------------------------------------------------------
void Cell_IO::click(bool state)
{
    QPushButton *btn = (QPushButton *)sender();
    int x = btn->property("X").toInt();
    int y = btn->property("Y").toInt();
    emit push(x, y, state);
}
//--------------------------------------------------------------------------------
