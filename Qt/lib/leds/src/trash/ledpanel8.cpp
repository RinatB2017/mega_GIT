//--------------------------------------------------------------------------------
#include <QtGui>
//--------------------------------------------------------------------------------
#include "ledpanel8.h"
//--------------------------------------------------------------------------------
LedPanel8::LedPanel8(QWidget *parent) :
    QFrame(parent),
    logger(this)
{
    int size = 32;

    logger.info("LedPanel8 started!");

    QHBoxLayout *hbox = new QHBoxLayout(this);

    led0 = new QFrame;
    led1 = new QFrame;
    led2 = new QFrame;
    led3 = new QFrame;
    led4 = new QFrame;
    led5 = new QFrame;
    led6 = new QFrame;
    led7 = new QFrame;

    led0->setFrameShape(StyledPanel);
    led0->setFrameShadow(Sunken);
    led1->setFrameShape(StyledPanel);
    led1->setFrameShadow(Sunken);
    led2->setFrameShape(StyledPanel);
    led2->setFrameShadow(Sunken);
    led3->setFrameShape(StyledPanel);
    led3->setFrameShadow(Sunken);
    led4->setFrameShape(StyledPanel);
    led4->setFrameShadow(Sunken);
    led5->setFrameShape(StyledPanel);
    led5->setFrameShadow(Sunken);
    led6->setFrameShape(StyledPanel);
    led6->setFrameShadow(Sunken);
    led7->setFrameShape(StyledPanel);
    led7->setFrameShadow(Sunken);

    led0->setToolTip("0");
    led1->setToolTip("1");
    led2->setToolTip("2");
    led3->setToolTip("3");
    led4->setToolTip("4");
    led5->setToolTip("5");
    led6->setToolTip("6");
    led7->setToolTip("7");

    led0->setMinimumSize(size, size);
    led1->setMinimumSize(size, size);
    led2->setMinimumSize(size, size);
    led3->setMinimumSize(size, size);
    led4->setMinimumSize(size, size);
    led5->setMinimumSize(size, size);
    led6->setMinimumSize(size, size);
    led7->setMinimumSize(size, size);

    hbox->addWidget(led7);
    hbox->addWidget(led6);
    hbox->addWidget(led5);
    hbox->addWidget(led4);
    hbox->addWidget(led3);
    hbox->addWidget(led2);
    hbox->addWidget(led1);
    hbox->addWidget(led0);
    hbox->addStretch();

    drawValue(0);

    setFrameStyle(QFrame::Box | QFrame::Raised);
}
//--------------------------------------------------------------------------------
LedPanel8::~LedPanel8()
{
    delete led0;
    delete led1;
    delete led2;
    delete led3;
    delete led4;
    delete led5;
    delete led6;
    delete led7;
}
//--------------------------------------------------------------------------------
void LedPanel8::drawValue(unsigned char data)
{
    led7->setStyleSheet((data & 0x80) ? "background:green" : "");
    led6->setStyleSheet((data & 0x40) ? "background:green" : "");
    led5->setStyleSheet((data & 0x20) ? "background:green" : "");
    led4->setStyleSheet((data & 0x10) ? "background:green" : "");
    led3->setStyleSheet((data & 0x08) ? "background:green" : "");
    led2->setStyleSheet((data & 0x04) ? "background:green" : "");
    led1->setStyleSheet((data & 0x02) ? "background:green" : "");
    led0->setStyleSheet((data & 0x01) ? "background:green" : "");
}
//--------------------------------------------------------------------------------
