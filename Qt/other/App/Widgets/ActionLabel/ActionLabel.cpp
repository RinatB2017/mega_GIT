#include "ActionLabel.h"

#include <QStyle>
#include <QMouseEvent>

ActionLabel::ActionLabel(QWidget *parent) :
    QLabel(parent)
{
    MouseOver = false;
}

void ActionLabel::enterEvent (QEvent*)
{
    MouseOver = true;
    this->style()->polish(this);
}

void ActionLabel::leaveEvent (QEvent*)
{
    MouseOver = false;
    this->style()->polish(this);
}

void ActionLabel::mousePressEvent (QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        emit clicked();
}

bool ActionLabel::mouseOver ()
{
    return MouseOver;
}
