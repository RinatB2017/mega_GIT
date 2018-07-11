//--------------------------------------------------------------------------------
#ifndef LEDPANEL8_H
#define LEDPANEL8_H
//--------------------------------------------------------------------------------
#include <QtGui>
//--------------------------------------------------------------------------------
#include "Logging.h"
//--------------------------------------------------------------------------------
class LedPanel8 : public QFrame
{
    Q_OBJECT
public:
    explicit LedPanel8(QWidget *parent = 0);
    ~LedPanel8();

signals:

public slots:
    void drawValue(unsigned char data);

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
};
//--------------------------------------------------------------------------------
#endif // LEDPANEL8_H
