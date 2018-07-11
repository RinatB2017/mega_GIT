//--------------------------------------------------------------------------------
#include <QtGui>
//--------------------------------------------------------------------------------
#include "ledpanel4.h"
//--------------------------------------------------------------------------------
LedPanel4::LedPanel4(QWidget *parent) :
    QFrame(parent),
    logger(this)
{
    int size = 32;

    logger.info("LedPanel4 started!");

    QHBoxLayout *hbox = new QHBoxLayout(this);

    led0 = new QFrame;
    led1 = new QFrame;
    led2 = new QFrame;
    led3 = new QFrame;

    led0->setFrameShape(StyledPanel);
    led0->setFrameShadow(Sunken);
    led1->setFrameShape(StyledPanel);
    led1->setFrameShadow(Sunken);
    led2->setFrameShape(StyledPanel);
    led2->setFrameShadow(Sunken);
    led3->setFrameShape(StyledPanel);
    led3->setFrameShadow(Sunken);

    led0->setToolTip("0");
    led1->setToolTip("1");
    led2->setToolTip("2");
    led3->setToolTip("3");

    led0->setMinimumSize(size, size);
    led1->setMinimumSize(size, size);
    led2->setMinimumSize(size, size);
    led3->setMinimumSize(size, size);

    hbox->addWidget(led3);
    hbox->addWidget(led2);
    hbox->addWidget(led1);
    hbox->addWidget(led0);
    hbox->addStretch();

    drawValue(0);

    setFrameStyle(QFrame::Box | QFrame::Raised);
}
//--------------------------------------------------------------------------------
LedPanel4::~LedPanel4()
{
    delete led0;
    delete led1;
    delete led2;
    delete led3;
}
//--------------------------------------------------------------------------------
void LedPanel4::drawValue(unsigned char data)
{
    led3->setStyleSheet((data & 0x08) ? "background:green" : "");
    led2->setStyleSheet((data & 0x04) ? "background:green" : "");
    led1->setStyleSheet((data & 0x02) ? "background:green" : "");
    led0->setStyleSheet((data & 0x01) ? "background:green" : "");
}
//--------------------------------------------------------------------------------
