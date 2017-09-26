//--------------------------------------------------------------------------------
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
//--------------------------------------------------------------------------------
#include "labelcombobox.hpp"
//--------------------------------------------------------------------------------
LabelComboBox::LabelComboBox(const QString &name_frame,
                             const QString &name_label,
                             const QStringList &sl_combobox,
                             int index,
                             int maxw,
                             QWidget* parent)
    : QFrame(parent)
{
    n_frame = name_frame;
    n_label = name_label;
    init_index = index;
    flag_log = true;

    hbox = new QHBoxLayout;
    label = new QLabel;
    label->setText(name_label);

    combobox = new QComboBox;
    combobox->setMaximumWidth(maxw);
    combobox->addItems(sl_combobox);
    combobox->setCurrentIndex(index);
    connect(combobox,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(change_state()));

    hbox->setMargin(0);
    hbox->addWidget(label);
    hbox->addStretch();
    hbox->addWidget(combobox);
    setLayout(hbox);
}
//--------------------------------------------------------------------------------
void LabelComboBox::enable(bool state)
{
    combobox->setEnabled(state);
}
//--------------------------------------------------------------------------------
void LabelComboBox::init(void)
{
    //    qDebug() << n_frame << " " << n_label;
    flag_log = false;
    combobox->setCurrentIndex(init_index);
    flag_log = true;
}
//--------------------------------------------------------------------------------
void LabelComboBox::change_state(void)
{
    if(!flag_log) return;
    emit log(QString(QLatin1String("%1 %2:%3")).arg(n_frame).arg(n_label).arg(combobox->currentText()));
    emit click_state();
}
//--------------------------------------------------------------------------------
void LabelComboBox::set_name(const QString &name)
{
    label->setText(name);
}
//--------------------------------------------------------------------------------
QString LabelComboBox::get_name(void)
{
    return label->text();
}
//--------------------------------------------------------------------------------
int LabelComboBox::set_index(int index)
{
    if(index<0) return -1;
    if(index>combobox->maxCount()) return -1;

    combobox->setCurrentIndex(index - 1);
    //    Log::print_log(name+" cостояние изменено: "+combobox->currentText());
    return 0;
}    
//--------------------------------------------------------------------------------
unsigned char LabelComboBox::get_index(void)
{
    return combobox->currentIndex()+1;
}
//--------------------------------------------------------------------------------
QString LabelComboBox::get_text(void)
{
    return combobox->currentText();
}
//--------------------------------------------------------------------------------
