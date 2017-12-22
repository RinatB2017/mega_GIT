#ifndef LED_H
#define LED_H

#include <QWidget>

class QColor;

class LED : public QWidget
{
    Q_OBJECT

public:
    LED(QWidget *parent = 0);
    ~LED();

    QColor color() const;
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

public slots:
    void setColor(const QColor &color);
    void toggle();
    void turnOn(bool on=true);
    void turnOff(bool off=true);

protected:
    void paintEvent(QPaintEvent *);
    int ledWidth() const;

private:
    struct Private;
    Private* const m_d;
};

#endif // LED_H
