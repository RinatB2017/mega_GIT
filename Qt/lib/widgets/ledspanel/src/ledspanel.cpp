//--------------------------------------------------------------------------------
#include <QGridLayout>
#include <QFrame>
#include <QLabel>
//--------------------------------------------------------------------------------
#include "ledspanel.hpp"
//--------------------------------------------------------------------------------
#define LED_ON_RED    "background:red"
#define LED_ON_GREEN  "background:green"
#define LED_ON_YELLOW "background:yellow"
#define LED_OFF ""
//--------------------------------------------------------------------------------
union STATE
{
    unsigned char value;
    struct
    {
        unsigned bit0:1;
        unsigned bit1:1;
        unsigned bit2:1;
        unsigned bit3:1;
        unsigned bit4:1;
        unsigned bit5:1;
        unsigned bit6:1;
        unsigned bit7:1;
    } bits;
};
//--------------------------------------------------------------------------------
LedsPanel::LedsPanel(QWidget *parent) :
    QFrame(parent)
{
    int n;

    vbox = new QVBoxLayout(this);

    main_frame = new QGridLayout;

    for(n=0; n<8; n++)
    {
        frame[n] = new QFrame;
        frame[n]->setFixedSize(24, 24);
        frame[n]->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);

        label[n] = new QLabel;
        label[n]->setText(QString("bit%1").arg(n));
    }

    for(n=0; n<8; n++)
    {
        main_frame->addWidget(frame[n], n, 0);
        main_frame->addWidget(label[n], n, 1);
    }

    vbox->addLayout(main_frame);
    vbox->addStretch();

    setLayout(vbox);
    setFrameStyle(QFrame::StyledPanel| QFrame::Raised);
}
//--------------------------------------------------------------------------------
LedsPanel::~LedsPanel()
{
    int n;

    for(n=0; n<8; n++)
    {
        delete frame[n];
        delete label[n];
    }

    delete main_frame;
    delete vbox;
}
//--------------------------------------------------------------------------------
void LedsPanel::set_text(unsigned int index, const QString &text)
{
    if(text.isEmpty())
    {
        return;
    }
    if(index > 7)
    {
        return;
    }
    label[index]->setText(text);
}
//--------------------------------------------------------------------------------
void LedsPanel::set_value(unsigned int index, bool state)
{
    if(index > 7)
    {
        return;
    }
    frame[index]->setStyleSheet(state ? LED_ON_GREEN : LED_OFF);
}
//--------------------------------------------------------------------------------
void LedsPanel::draw_value(unsigned char data)
{
    union STATE state;

    state.value = data;
    set_value(0, state.bits.bit0);
    set_value(1, state.bits.bit1);
    set_value(2, state.bits.bit2);
    set_value(3, state.bits.bit3);
    set_value(4, state.bits.bit4);
    set_value(5, state.bits.bit5);
    set_value(6, state.bits.bit6);
    set_value(7, state.bits.bit7);
}
//--------------------------------------------------------------------------------
