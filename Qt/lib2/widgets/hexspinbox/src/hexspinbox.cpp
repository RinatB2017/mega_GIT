//--------------------------------------------------------------------------------
#include "hexspinbox.hpp"
//--------------------------------------------------------------------------------
HexSpinBox::HexSpinBox(QWidget *parent)
    : QSpinBox(parent)
{
    setRange(0,0xFFFF);
#ifdef HEX8
    validator = new QRegExpValidator(QRegExp(QLatin1String("[0-9A-Fa-f]{1,2}")), this);
#endif
#ifdef HEX16
    validator = new QRegExpValidator(QRegExp(QLatin1String("[0-9A-Fa-f]{1,4}")), this);
#endif
#ifdef HEX32
    validator = new QRegExpValidator(QRegExp(QLatin1String("[0-9A-Fa-f]{1,8}")), this);
#endif
}
//--------------------------------------------------------------------------------
QValidator::State HexSpinBox::validate(QString &text, int &pos) const
{
    return validator->validate(text, pos);
}
//--------------------------------------------------------------------------------
QString HexSpinBox::textFromValue(int value) const
{
#ifdef HEX8
    return QString("%1").arg(value, 2, 16, QChar('0')).toUpper();
#endif
#ifdef HEX16
    return QString("%1").arg(value, 4, 16, QChar('0')).toUpper();
#endif
#ifdef HEX32
    return QString("%1").arg(value, 8, 16, QChar('0')).toUpper();
#endif
}
//--------------------------------------------------------------------------------
int HexSpinBox::valueFromText(const QString &text) const
{
    bool ok = false;
    return text.toInt(&ok, 16);
}
//--------------------------------------------------------------------------------
