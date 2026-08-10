//--------------------------------------------------------------------------------
#include <QVBoxLayout>
#include <QLCDNumber>
#include <qwt_knob.h>
//--------------------------------------------------------------------------------
#include "lcd_knob.hpp"
//--------------------------------------------------------------------------------
LCD_Knob::LCD_Knob(QWidget *parent) :
    QFrame(parent)
{
    lcd  = new QLCDNumber(this);
    lcd->setMinimumHeight(32);
    lcd->setSegmentStyle(QLCDNumber::Flat);
//    lcd->setStyleSheet("background-color:black; color:green;");

    knob = new QwtKnob(this);
    knob->setScaleMaxMajor(10);
    knob->setScaleMaxMinor(10);
    knob->setKnobStyle(QwtKnob::Sunken);

    vbox = new QVBoxLayout(this);

    vbox->addWidget(lcd);
    vbox->addWidget(knob);

    setLayout(vbox);

    connect(knob, SIGNAL(valueChanged(double)), lcd, SLOT(display(double)));
    connect(knob, SIGNAL(valueChanged(double)), this, SIGNAL(valueChanged(double)));

    knob->setWhatsThis(tr("Это крутилка"));
    knob->setToolTip(tr("Это мега крутилка"));

    lcd->setWhatsThis(tr("Это индикатор"));
}
//--------------------------------------------------------------------------------
LCD_Knob::~LCD_Knob()
{
    if(lcd)  delete lcd;
    if(knob) delete knob;
    if(vbox) delete vbox;
}
//--------------------------------------------------------------------------------
void LCD_Knob::setRange(double min, double max)
{
    knob->setLowerBound(min);
    knob->setUpperBound(max);
}
//--------------------------------------------------------------------------------
void LCD_Knob::setScaleMaxMajor(int ticks)
{
    knob->setScaleMaxMajor(ticks);
}
//--------------------------------------------------------------------------------
void LCD_Knob::setScaleMaxMinor(int ticks)
{
    knob->setScaleMaxMinor(ticks);
}
//--------------------------------------------------------------------------------
void LCD_Knob::setValue(double value)
{
    knob->setValue(value);
}
//--------------------------------------------------------------------------------
