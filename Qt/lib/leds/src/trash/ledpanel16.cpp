//--------------------------------------------------------------------------------
#include <QtGui>
//--------------------------------------------------------------------------------
#include "ledpanel16.h"
//--------------------------------------------------------------------------------
LedPanel16::LedPanel16(QWidget *parent) :
    QFrame(parent),
    logger(this)
{
    int size = 32;

    logger.info("LedPanel16 started!");

    QHBoxLayout *hbox = new QHBoxLayout(this);

    led0 = new QFrame;
    led1 = new QFrame;
    led2 = new QFrame;
    led3 = new QFrame;
    led4 = new QFrame;
    led5 = new QFrame;
    led6 = new QFrame;
    led7 = new QFrame;
    led8 = new QFrame;
    led9 = new QFrame;
    led10 = new QFrame;
    led11 = new QFrame;
    led12 = new QFrame;
    led13 = new QFrame;
    led14 = new QFrame;
    led15 = new QFrame;

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
    led8->setFrameShape(StyledPanel);
    led8->setFrameShadow(Sunken);
    led9->setFrameShape(StyledPanel);
    led9->setFrameShadow(Sunken);
    led10->setFrameShape(StyledPanel);
    led10->setFrameShadow(Sunken);
    led11->setFrameShape(StyledPanel);
    led11->setFrameShadow(Sunken);
    led12->setFrameShape(StyledPanel);
    led12->setFrameShadow(Sunken);
    led13->setFrameShape(StyledPanel);
    led13->setFrameShadow(Sunken);
    led14->setFrameShape(StyledPanel);
    led14->setFrameShadow(Sunken);
    led15->setFrameShape(StyledPanel);
    led15->setFrameShadow(Sunken);

    led0->setToolTip("0");
    led1->setToolTip("1");
    led2->setToolTip("2");
    led3->setToolTip("3");
    led4->setToolTip("4");
    led5->setToolTip("5");
    led6->setToolTip("6");
    led7->setToolTip("7");
    led8->setToolTip("8");
    led9->setToolTip("9");
    led10->setToolTip("10");
    led11->setToolTip("11");
    led12->setToolTip("12");
    led13->setToolTip("13");
    led14->setToolTip("14");
    led15->setToolTip("15");

    led0->setMinimumSize(size, size);
    led1->setMinimumSize(size, size);
    led2->setMinimumSize(size, size);
    led3->setMinimumSize(size, size);
    led4->setMinimumSize(size, size);
    led5->setMinimumSize(size, size);
    led6->setMinimumSize(size, size);
    led7->setMinimumSize(size, size);
    led8->setMinimumSize(size, size);
    led9->setMinimumSize(size, size);
    led10->setMinimumSize(size, size);
    led11->setMinimumSize(size, size);
    led12->setMinimumSize(size, size);
    led13->setMinimumSize(size, size);
    led14->setMinimumSize(size, size);
    led15->setMinimumSize(size, size);

    QLabel *label = new QLabel;
    label->setFrameStyle(QFrame::VLine);
    //label->setLineWidth(3);

    hbox->addWidget(led15);
    hbox->addWidget(led14);
    hbox->addWidget(led13);
    hbox->addWidget(led12);
    hbox->addWidget(led11);
    hbox->addWidget(led10);
    hbox->addWidget(led9);
    hbox->addWidget(led8);

    hbox->addWidget(label);

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
LedPanel16::~LedPanel16()
{
    delete led0;
    delete led1;
    delete led2;
    delete led3;
    delete led4;
    delete led5;
    delete led6;
    delete led7;
    delete led8;
    delete led9;
    delete led10;
    delete led11;
    delete led12;
    delete led13;
    delete led14;
    delete led15;
}
//--------------------------------------------------------------------------------
void LedPanel16::drawValue(unsigned short data)
{
    led15->setStyleSheet((data & 0x8000) ? "background:green" : "");
    led14->setStyleSheet((data & 0x4000) ? "background:green" : "");
    led13->setStyleSheet((data & 0x2000) ? "background:green" : "");
    led12->setStyleSheet((data & 0x1000) ? "background:green" : "");
    led11->setStyleSheet((data & 0x0800) ? "background:green" : "");
    led10->setStyleSheet((data & 0x0400) ? "background:green" : "");
    led9->setStyleSheet((data & 0x0200) ? "background:green" : "");
    led8->setStyleSheet((data & 0x0100) ? "background:green" : "");
    led7->setStyleSheet((data & 0x0080) ? "background:green" : "");
    led6->setStyleSheet((data & 0x0040) ? "background:green" : "");
    led5->setStyleSheet((data & 0x0020) ? "background:green" : "");
    led4->setStyleSheet((data & 0x0010) ? "background:green" : "");
    led3->setStyleSheet((data & 0x0008) ? "background:green" : "");
    led2->setStyleSheet((data & 0x0004) ? "background:green" : "");
    led1->setStyleSheet((data & 0x0002) ? "background:green" : "");
    led0->setStyleSheet((data & 0x0001) ? "background:green" : "");
}
//--------------------------------------------------------------------------------
