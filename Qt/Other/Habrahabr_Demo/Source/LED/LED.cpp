#include "LED.h"

#include <QtGui>

struct LED::Private
{
public:
    Private()
        : darkerFactor(300), color(Qt::green), isOn(true)
    {}

    int darkerFactor;
    QColor color;
    bool isOn;
};

LED::LED(QWidget *parent) : QWidget(parent), m_d(new Private)
{
}

LED::~LED()
{
    delete m_d;
}

QColor LED::color() const
{
    return m_d->color;
}

void LED::setColor(const QColor &color)
{
    if (m_d->color == color)
        return;
    update();
}

QSize LED::sizeHint() const
{
    return QSize(20, 20);
}

QSize LED::minimumSizeHint() const
{
    return QSize(16, 16);
}

void LED::toggle()
{
    m_d->isOn = !m_d->isOn;
    update();
}

void LED::turnOn(bool on)
{
    m_d->isOn = on;
    update();
}

void LED::turnOff(bool off)
{
    turnOn(!off);
}

void LED::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(*event)

    int width = ledWidth();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor color = m_d->isOn ? m_d->color
                             : m_d->color.darker(m_d->darkerFactor);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(color);
    painter.setBrush(brush);
    // Рисуем поверхность
    painter.drawEllipse(1, 1, width-1, width-1);

    QPen pen;
    pen.setWidth(2);

    int pos = width / 5 + 1;
    int lightWidth = width * 2 / 3;
    int lightQuote = 130 * 2 / (lightWidth ? lightWidth : 1) + 100;

    // Рисуем окружность
    while(lightWidth)
    {
        color = color.lighter(lightQuote);
        pen.setColor(color);
        painter.setPen(pen);
        painter.drawEllipse(pos, pos, lightWidth, lightWidth);
        lightWidth--;

        if(!lightWidth)
        {
            break;
        }

        painter.drawEllipse(pos, pos, lightWidth, lightWidth);
        lightWidth--;

        if(!lightWidth)
        {
            break;
        }

        painter.drawEllipse(pos, pos, lightWidth, lightWidth);
        pos++;
        lightWidth--;
    }

    // Рисуем границу
    painter.setBrush(Qt::NoBrush);

    int angle = -720;
    color = palette().color(QPalette::Light);

    for (int arc=120; arc<2880; arc+=240)
    {
        pen.setColor(color);
        painter.setPen(pen);
        int w = width - pen.width()/2;
        painter.drawArc(pen.width()/2, pen.width()/2, w, w, angle+arc, 240);
        painter.drawArc(pen.width()/2, pen.width()/2, w, w, angle-arc, 240);
        color = color.darker(110);
    }
}

int LED::ledWidth() const
{
    int width = qMin(this->width(), this->height());
    width -= 2;
    return width > 0 ? width : 0;
}
