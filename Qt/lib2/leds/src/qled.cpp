//--------------------------------------------------------------------------------
#include <QPainter>
#include "qled.hpp"
//--------------------------------------------------------------------------------
QLed::QLed(unsigned int width,
           unsigned int height,
           const QString &tooltip,
           const QColor &color_off,
           const QColor &color_on,
           QWidget *parent) :
    QWidget(parent)
{
    this->color_off = color_off;
    this->color_on = color_on;

    if(width < 16) width = 16;
    if(height < 16) height = 16;
    setFixedSize(static_cast<int>(width),
                 static_cast<int>(height));
    setToolTip(tooltip);

    current_color = color_off;
    update();
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
