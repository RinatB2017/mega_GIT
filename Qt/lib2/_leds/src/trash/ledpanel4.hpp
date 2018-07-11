//--------------------------------------------------------------------------------
#ifndef LEDPANEL4_H
#define LEDPANEL4_H
//--------------------------------------------------------------------------------
#include <QtGui>
//--------------------------------------------------------------------------------
#include "Logging.h"
//--------------------------------------------------------------------------------
class LedPanel4 : public QFrame
{
    Q_OBJECT
public:
    explicit LedPanel4(QWidget *parent = 0);
    ~LedPanel4();

signals:

public slots:
    void drawValue(unsigned char data);

private:
    Logger logger;
    QFrame *led0;
    QFrame *led1;
    QFrame *led2;
    QFrame *led3;
};
//--------------------------------------------------------------------------------
#endif // LEDPANEL4_H
