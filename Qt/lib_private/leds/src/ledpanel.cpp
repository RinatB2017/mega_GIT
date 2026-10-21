//--------------------------------------------------------------------------------
#include <QHBoxLayout>
//--------------------------------------------------------------------------------
#include "structures.hpp"
#include "ledpanel.hpp"
#include "qled.hpp"
//--------------------------------------------------------------------------------
LedPanel::LedPanel(QWidget *parent) :
    QFrame(parent)
{
    unsigned int n;

    count = 8;

    emit log("LedPanel started!");
    hbox = new QHBoxLayout;
    for(n=0; n<count; n++)
    {
        led[n] = new QLed();
        led[n]->set_size(32, 32);
        led[n]->set_tooltip(QString::number(n));
        //TODO цвета диодов
        led[n]->set_color_on(Qt::red);
        // led[n]->set_color_on(Qt::green);
        // led[n]->set_color_on(Qt::blue);
    }
    for(n=count; n>0; n--)
    {
        hbox->addWidget(led[n-1]);
    }
    hbox->addStretch();

    setLayout(hbox);
    setFrameStyle(QFrame::Box | QFrame::Raised);
}
//--------------------------------------------------------------------------------
LedPanel::LedPanel(unsigned int max_leds,
                   unsigned int width_leds,
                   unsigned int height_leds,
                   QWidget *parent) :
    QFrame(parent)
{
    unsigned int n;

    if(max_leds > MAXLEDS)
    {
        emit log(QString("%1 > MAXLEDS").arg(max_leds));
        max_leds = MAXLEDS;
    }
    count = max_leds;

    emit log("LedPanel started!");
    hbox = new QHBoxLayout;
    for(n=0; n<count; n++)
    {
        led[n] = new QLed();
        led[n]->set_size(width_leds, height_leds);
        led[n]->set_tooltip(QString::number(n));
        led[n]->set_color_on(Qt::green);
    }
    for(n=count; n>0; n--)
    {
        hbox->addWidget(led[n-1]);
    }
    hbox->addStretch();

    setLayout(hbox);
    setFrameStyle(QFrame::Box | QFrame::Raised);
}
//--------------------------------------------------------------------------------
LedPanel::~LedPanel()
{
    unsigned int n;
    for(n=0; n<count; n++) delete led[n];
    delete hbox;
}
//--------------------------------------------------------------------------------
void LedPanel::drawValue(unsigned long data)
{
    unsigned int n;
    unsigned int mask;

    for(n=0; n<count; n++)
    {
        mask = 1 << n;
        led[n]->set_state(data & mask);
    }
}
//--------------------------------------------------------------------------------
void LedPanel::set_u8_value(uint8_t value)
{
    U_8 u8;
    u8.value = value;
    led[7]->set_state(u8.bites.bit7);
    led[6]->set_state(u8.bites.bit6);
    led[5]->set_state(u8.bites.bit5);
    led[4]->set_state(u8.bites.bit4);
    led[3]->set_state(u8.bites.bit3);
    led[2]->set_state(u8.bites.bit2);
    led[1]->set_state(u8.bites.bit1);
    led[0]->set_state(u8.bites.bit0);
}
//--------------------------------------------------------------------------------
uint8_t LedPanel::get_u8_value()
{
    U_8 u8;
    u8.bites.bit7 = led[7]->get_state();
    u8.bites.bit6 = led[6]->get_state();
    u8.bites.bit5 = led[5]->get_state();
    u8.bites.bit4 = led[4]->get_state();
    u8.bites.bit3 = led[3]->get_state();
    u8.bites.bit2 = led[2]->get_state();
    u8.bites.bit1 = led[1]->get_state();
    u8.bites.bit0 = led[0]->get_state();

    return u8.value;
}
//--------------------------------------------------------------------------------
void LedPanel::set_tool_tips(const QStringList &sl)
{
    if(sl.isEmpty())
    {
        return;
    }
    if(sl.count() != count)
    {
        return;
    }
    for(int n=(int)count-1; n>=0; n--)
    {
        led[n]->setToolTip(sl.at(n));
    }
}
//--------------------------------------------------------------------------------
void LedPanel::set_other_color(uint8_t mask_value)
{
    U_8 mask;
    QColor other_color = Qt::yellow;

    mask.value = mask_value;
    if(mask.bites.bit0) led[0]->set_color_on(other_color);
    if(mask.bites.bit1) led[1]->set_color_on(other_color);
    if(mask.bites.bit2) led[2]->set_color_on(other_color);
    if(mask.bites.bit3) led[3]->set_color_on(other_color);
    if(mask.bites.bit4) led[4]->set_color_on(other_color);
    if(mask.bites.bit5) led[5]->set_color_on(other_color);
    if(mask.bites.bit6) led[6]->set_color_on(other_color);
    if(mask.bites.bit7) led[7]->set_color_on(other_color);
}
//--------------------------------------------------------------------------------
int LedPanel::setLedToolTip(unsigned int position, const QString &string)
{
    if(position > count)
    {
        emit log("position too big");
        return -1;
    }
    led[position]->setToolTip(string);
    return 0;
}
//--------------------------------------------------------------------------------
void LedPanel::setEnabled(bool state)
{
    unsigned int n;
    for(n=0; n<count; n++) led[n]->setEnabled(state);
}
//--------------------------------------------------------------------------------
