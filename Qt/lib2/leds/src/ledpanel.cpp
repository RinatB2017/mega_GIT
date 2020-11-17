//--------------------------------------------------------------------------------
#include <QHBoxLayout>
//--------------------------------------------------------------------------------
#include "qled.hpp"
#include "ledpanel.hpp"
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
