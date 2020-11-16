//--------------------------------------------------------------------------------
#include <QPainter>
#include "qled.hpp"
//--------------------------------------------------------------------------------
QLed::QLed(QWidget *parent) :
    QWidget(parent)
{

}
//--------------------------------------------------------------------------------
void QLed::setState(bool state)
{
    if(state)
    {
        current_color = color_on;
    }
    else
    {
        current_color = color_off;
    }
    update();
}
//--------------------------------------------------------------------------------
void QLed::set_size(int w, int h)
{
    setFixedSize(w, h);
}
//--------------------------------------------------------------------------------
void QLed::set_tooltip(const QString &text)
{
    tooltip = text;
}
//--------------------------------------------------------------------------------
void QLed::set_color(QColor color)
{
    current_color = color;
    update();
}
//--------------------------------------------------------------------------------
void QLed::set_color_on(QColor color)
{
    color_on = color;
    update();
}
//--------------------------------------------------------------------------------
void QLed::set_color_off(QColor color)
{
    color_off = color;
    update();
}
//--------------------------------------------------------------------------------
void QLed::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(0, 0, width(), height(), Qt::transparent);
    painter.setPen(QPen(Qt::black));
    painter.setBrush(QBrush(current_color));
    painter.drawEllipse(QPointF(width() / 2.0, height() / 2.0), width() / 2.0 - 1.0, height() / 2.0 - 1.0);
}
//--------------------------------------------------------------------------------
