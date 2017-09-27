//--------------------------------------------------------------------------------
#include "hexspinbox.hpp"
//--------------------------------------------------------------------------------
HexSpinBox::HexSpinBox(QWidget *parent)
    : QSpinBox(parent)
{
//    setRange(0,0xFFFF);
    validator = new QRegExpValidator(QRegExp(QLatin1String("[0-9A-Fa-f]{1,8}")), this);
}
//--------------------------------------------------------------------------------
QValidator::State HexSpinBox::validate(QString &text, int &pos) const
{
    return validator->validate(text, pos);
}
//--------------------------------------------------------------------------------
QString HexSpinBox::textFromValue(int value) const
{
    //return QString::number(value, 16).toUpper();
    return QString("%1").arg(value, 2, 16, QChar('0')).toUpper();
}
//--------------------------------------------------------------------------------
int HexSpinBox::valueFromText(const QString &text) const
{
    bool ok;
    return text.toInt(&ok, 16);
}
//--------------------------------------------------------------------------------
