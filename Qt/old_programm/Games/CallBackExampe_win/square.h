#ifndef SQUARE_H
#define SQUARE_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QTimer>
#include <QPointF>

class Square : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Square(QObject *parent = 0);
    ~Square();
    // Функция для установки callback функции
    void setCallbackFunc(void (*func) (QPointF point));

signals:

public slots:

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QTimer *timer;  // игровой таймер
    // Указатель на callback функцию
    void (*callbackFunc)(QPointF point);

private slots:
    void slotTimer();   // слот игрового таймера

};

#endif // SQUARE_H
