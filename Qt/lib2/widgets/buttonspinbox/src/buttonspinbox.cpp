//--------------------------------------------------------------------------------
#include <QHBoxLayout>
#include <QPushButton>
//--------------------------------------------------------------------------------
#include "hexspinbox.hpp"
#include "buttonspinbox.hpp"
//--------------------------------------------------------------------------------
ButtonSpinBox::ButtonSpinBox(QWidget* parent)
    : QFrame(parent)
{
    hbox = new QHBoxLayout;
    button = new QPushButton;
    spinbox = new HexSpinBox;

    hbox->setMargin(0);
    hbox->addWidget(button);
    hbox->addStretch();
    hbox->addWidget(spinbox);
    setLayout(hbox);
    connect(button,SIGNAL(clicked()),this,SIGNAL(value_changed()));
}
//--------------------------------------------------------------------------------
void ButtonSpinBox::enable(bool state)
{
    button->setEnabled(state);
    spinbox->setEnabled(state);
}
//--------------------------------------------------------------------------------
void ButtonSpinBox::init(const QString &text, int min, int max, int value)
{
    button->setText(text);
    spinbox->setRange(min,max);
    spinbox->setValue(value);
}
//--------------------------------------------------------------------------------
int ButtonSpinBox::set_name(const QString &text_button)
{
    button->setText(text_button);
    return 0;
}
//--------------------------------------------------------------------------------
int ButtonSpinBox::set_value(int value)
{
    spinbox->setValue(value);
    return 0;
}
//--------------------------------------------------------------------------------
int ButtonSpinBox::get_value(void)
{
    return spinbox->value();
}
//--------------------------------------------------------------------------------
