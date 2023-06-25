//
// https://stackoverflow.com/questions/26581444/qspinbox-with-unsigned-int-for-hex-input
//

#include "hexspinbox.hpp"

HexSpinBox::HexSpinBox(QWidget *parent) : QSpinBox(parent), m_maxRange(maximum())
{
    setDisplayIntegerBase(16);
}

void HexSpinBox::setRange(unsigned int min,
                          unsigned int max)
{
    m_maxRange = max;
    if (m_maxRange <= INT_MAX) {
        QSpinBox::setRange(min, int(m_maxRange));
    } else {
        QSpinBox::setRange(INT_MIN, INT_MAX);
    }
}

QString HexSpinBox::textFromValue(int value) const
{
    int fillField = 0;
    if (m_fillField) {
        uint m = m_maxRange;
        while (m) {
            m >>= 4;
            ++fillField;
        }
    }
    return QString("%1").arg(u(value), fillField, 16, QLatin1Char('0')).toUpper();
}

int HexSpinBox::valueFromText(const QString &text) const
{
    return i(text.toUInt(nullptr, 16));
}

QValidator::State HexSpinBox::validate(QString &input, int &pos) const
{
    QString copy(input);
    QString pref = prefix();
    if (copy.startsWith(pref))
        copy.remove(pref);
    pos -= copy.size() - copy.trimmed().size();
    copy = copy.trimmed();
    if (copy.isEmpty())
        return QValidator::Intermediate;
    input = pref + copy.toUpper();
    bool okay;
    unsigned int val = copy.toUInt(&okay, 16);
    if (!okay || val > m_maxRange)
        return QValidator::Invalid;
    return QValidator::Acceptable;
}
