//--------------------------------------------------------------------------------
#ifndef LEDPANEL16_H
#define LEDPANEL16_H
//--------------------------------------------------------------------------------
#include <QtGui>
//--------------------------------------------------------------------------------
#include "Logging.h"
//--------------------------------------------------------------------------------
class LedPanel16 : public QFrame
{
    Q_OBJECT

public:
    explicit LedPanel16(QWidget *parent = 0);
    ~LedPanel16();

signals:

public slots:
    void drawValue(unsigned short data);

private:
    Logger logger;
    QFrame *led0;
    QFrame *led1;
    QFrame *led2;
    QFrame *led3;
    QFrame *led4;
    QFrame *led5;
    QFrame *led6;
    QFrame *led7;
    QFrame *led8;
    QFrame *led9;
    QFrame *led10;
    QFrame *led11;
    QFrame *led12;
    QFrame *led13;
    QFrame *led14;
    QFrame *led15;
};
//--------------------------------------------------------------------------------
#endif // LEDPANEL16_H
