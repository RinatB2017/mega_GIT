#include "square.h"
#include <windows.h>

Square::Square(QObject *parent) :
    QObject(parent), QGraphicsItem()
{
    // Инициализируем и настраиваем игровой таймер
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &Square::slotTimer);
    timer->start(1000/33);
}

Square::~Square()
{

}

QRectF Square::boundingRect() const
{
    return QRectF(-15,-15,30,30);
}

void Square::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::black);
    painter->setBrush(Qt::green);
    painter->drawRect(-15,-15,30,30);

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Square::slotTimer()
{
    // В зависимости от нажатых кнопок перемещаем квадрат по сцене
    if(GetAsyncKeyState('A')){
        this->setX(this->x() - 2);
    }
    if(GetAsyncKeyState('D')){
        this->setX(this->x() + 2);
    }
    if(GetAsyncKeyState('W')){
        this->setY(this->y() - 2);
    }
    if(GetAsyncKeyState('S')){
        this->setY(this->y() + 2);
    }
    // Вызываем callback функцию для передачи координат квадрата
    callbackFunc(this->pos());
}

void Square::setCallbackFunc(void (*func)(QPointF point))
{
    // Устанавливаем указатель на callback функцию
    callbackFunc = func;
}
