//--------------------------------------------------------------------------------
#include <QHBoxLayout>
#include <QLabel>
//--------------------------------------------------------------------------------
#include "labelbox.hpp"
//--------------------------------------------------------------------------------
LabelBox::LabelBox(const QString &name_label, QWidget* parent)
    : QFrame(parent)
{
    state = 0;

    name = name_label;
    hbox = new QHBoxLayout;
    label = new QLabel;
    label->setText(name_label);

    hbox->setMargin(0);
    hbox->addStretch(1);
    hbox->addWidget(label);
    hbox->addStretch(1);
    setLayout(hbox);
}
//--------------------------------------------------------------------------------
int LabelBox::set_name(const QString &text)
{
    label->setText(text);
    return 0;
}
//--------------------------------------------------------------------------------
void LabelBox::set_state(int state)
{
    switch(state)
    {
    case 0:
        //            setFrameStyle(QFrame::Box | QFrame::Sunken);
        //            setFrameStyle(QFrame::NoFrame);
        label->setStyleSheet(QLatin1String("color: red"));
        //            setStyleSheet("background: red");
        label->setText(QObject::tr("Radio direction closed"));
        this->state = 0;
        break;
    case 1:
        //            setFrameStyle(QFrame::Box | QFrame::Raised);
        label->setStyleSheet(QLatin1String("color: green"));
        //            setStyleSheet("background: green");
        label->setText(QObject::tr("Radio direction opened"));
        this->state = 1;
        break;
    }
}
//--------------------------------------------------------------------------------
int LabelBox::get_state(void)
{
    return state;
}
//--------------------------------------------------------------------------------
