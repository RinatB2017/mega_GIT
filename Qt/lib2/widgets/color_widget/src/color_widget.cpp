//--------------------------------------------------------------------------------
#include <QPainter>
#include "color_widget.hpp"
//--------------------------------------------------------------------------------
Color_widget::Color_widget(QWidget *parent) :
    QWidget(parent)
{

}
//--------------------------------------------------------------------------------
void Color_widget::set_color(QColor color)
{
    current_color = color;
    update();
}
//--------------------------------------------------------------------------------
void Color_widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::black));
    painter.setBrush(QBrush(current_color));
    painter.fillRect(0, 0, width(), height(), current_color);
    //     painter.drawEllipse(QPointF(width() / 2.0, height() / 2.0), width() / 2.0 - 1.0, height() / 2.0 - 1.0);
}
//--------------------------------------------------------------------------------
