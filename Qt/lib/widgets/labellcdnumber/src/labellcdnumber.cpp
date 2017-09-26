//--------------------------------------------------------------------------------
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QString>
#include <QLabel>
//--------------------------------------------------------------------------------
#include "labellcdnumber.hpp"
//--------------------------------------------------------------------------------
LabelLCDNumber::LabelLCDNumber(const QString &name_label,
                               int nDigits,
                               int value,
                               QWidget* parent)
    : QFrame(parent)
{
    name = name_label;

    hbox = new QHBoxLayout;
    label = new QLabel;
    label->setText(name_label);

    lcdnumber = new QLCDNumber;
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    lcdnumber->setNumDigits(nDigits);
#else
    lcdnumber->setDigitCount(nDigits);
#endif
    lcdnumber->display(value);
    lcdnumber->setSegmentStyle(QLCDNumber::Flat);

    hbox->setMargin(0);
    hbox->addWidget(label);
    hbox->addStretch();
    hbox->addWidget(lcdnumber);

    setLayout(hbox);
    connect(lcdnumber, SIGNAL(overflow()),this, SLOT(over()));
}
//--------------------------------------------------------------------------------
void LabelLCDNumber::over(void)
{
    emit log(tr("LCDNumber: too many number"));
}
//--------------------------------------------------------------------------------
void LabelLCDNumber::set_value(int value)
{
    lcdnumber->display(value);
}
//--------------------------------------------------------------------------------
int LabelLCDNumber::get_value(void)
{
    return lcdnumber->intValue();
}
//--------------------------------------------------------------------------------
