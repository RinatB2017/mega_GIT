//--------------------------------------------------------------------------------
#include "hexspinbox.hpp"
//--------------------------------------------------------------------------------
HexSpinBox::HexSpinBox(QWidget *parent)
    : QSpinBox(parent)
{
#ifdef HEX32
    setRange(0,0xFFFFFFFF);
    validator = new QRegExpValidator(QRegExp(QLatin1String("[0-9A-Fa-f]{1,8}")), this);
#elif HEX16
    setRange(0,0xFFFF);
    validator = new QRegExpValidator(QRegExp(QLatin1String("[0-9A-Fa-f]{1,4}")), this);
#else
    setRange(0,0xFF);
    validator = new QRegExpValidator(QRegExp(QLatin1String("[0-9A-Fa-f]{1,2}")), this);
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
#ifdef HEX32
    return QString("%1").arg(value, 8, 16, QChar('0')).toUpper();
#elif HEX16
    return QString("%1").arg(value, 4, 16, QChar('0')).toUpper();
#else
    return QString("%1").arg(value, 2, 16, QChar('0')).toUpper();
#endif
}
//--------------------------------------------------------------------------------
int HexSpinBox::valueFromText(const QString &text) const
{
    bool ok = false;
    return text.toInt(&ok, 16);
}
//--------------------------------------------------------------------------------
