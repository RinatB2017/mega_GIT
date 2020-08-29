/*********************************************************************************
** https://stackoverflow.com/questions/8383620/64bit-int-spin-box-in-qt         **
** modified: Bikbao Rinat Zinorovich                                            **
**********************************************************************************/
#include "QLongSpinBox.hpp"
//--------------------------------------------------------------------------------
QLongSpinBox::QLongSpinBox(QWidget *parent) :
    QAbstractSpinBox(parent)
{
    connect(lineEdit(), &QLineEdit::textEdited, this,   &QLongSpinBox::onEditFinished);
};
//--------------------------------------------------------------------------------
QLongSpinBox::~QLongSpinBox()
{

};
//--------------------------------------------------------------------------------
ulong QLongSpinBox::value() const
{
    return m_value;
};
//--------------------------------------------------------------------------------
ulong QLongSpinBox::minimum() const
{
    return m_minimum;
};
//--------------------------------------------------------------------------------
ulong QLongSpinBox::maximum() const
{
    return m_maximum;
};
//--------------------------------------------------------------------------------
void QLongSpinBox::setMinimum(ulong min)
{
    m_minimum = min;
}
//--------------------------------------------------------------------------------
void QLongSpinBox::setMaximum(ulong max)
{
    m_maximum = max;
}
//--------------------------------------------------------------------------------
void QLongSpinBox::setRange(ulong min, ulong max)
{
    setMinimum(min);
    setMaximum(max);
}
//--------------------------------------------------------------------------------
void QLongSpinBox::stepBy(int steps)
{
    auto new_value = m_value;
    if (steps < 0 && new_value + steps > new_value)
    {
        new_value = std::numeric_limits<ulong>::min();
    }
    else if (steps > 0 && new_value + steps < new_value)
    {
        new_value = std::numeric_limits<ulong>::max();
    }
    else
    {
        new_value += steps;
    }

    lineEdit()->setText(textFromValue(new_value));
    setValue(new_value);
}
//--------------------------------------------------------------------------------
#ifdef IN_DEC
QValidator::State QLongSpinBox::validate(QString &input, int &pos) const
{
    Q_UNUSED(pos);

    bool ok;
    qlonglong val = input.toLong(&ok);
    if (!ok)
        return QValidator::Invalid;

    if (val < m_minimum || val > m_maximum)
        return QValidator::Invalid;

    return QValidator::Acceptable;
}
#else
QValidator::State QLongSpinBox::validate(QString &input, int &pos) const
{
    Q_UNUSED(pos);

    QByteArray temp_ba;
    temp_ba.append(input);

    bool ok = false;
    ulong val = temp_ba.toLong(&ok, 16);
    if (!ok)
    {
        qDebug() << "!ok" << input;
        return QValidator::Invalid;
    }

    if (val < m_minimum || val > m_maximum)
    {
        qDebug() << "min|max" << val;
        return QValidator::Invalid;
    }

    return QValidator::Acceptable;
}
#endif
//--------------------------------------------------------------------------------
#ifdef IN_DEC
qlonglong QLongSpinBox::valueFromText(const QString &text) const
{
    bool ok = false;
    return text.toInt(&ok, 16);
}
#else
ulong QLongSpinBox::valueFromText(const QString &text) const
{
    bool ok = false;
    return text.toLong(&ok, 16);
}
#endif
//--------------------------------------------------------------------------------
#ifdef IN_DEC
QString QLongSpinBox::textFromValue(qlonglong val) const
{
    return QString::number(val);
}
#else
QString QLongSpinBox::textFromValue(ulong val) const
{
    return QString("%1").arg(val, 8, 16, QChar('0')).toUpper();
}
#endif
//--------------------------------------------------------------------------------
QAbstractSpinBox::StepEnabled QLongSpinBox::stepEnabled() const
{
    return StepUpEnabled | StepDownEnabled;
}
//--------------------------------------------------------------------------------
void QLongSpinBox::setValue(ulong val)
{
    if (m_value != val)
    {
        lineEdit()->setText(textFromValue(val));
        m_value = val;
    }
}
//--------------------------------------------------------------------------------
void QLongSpinBox::onEditFinished()
{
    QString input = lineEdit()->text();
    int pos = 0;
    if (QValidator::Acceptable == validate(input, pos))
        setValue(valueFromText(input));
    else
        lineEdit()->setText(textFromValue(m_value));
}
//--------------------------------------------------------------------------------
