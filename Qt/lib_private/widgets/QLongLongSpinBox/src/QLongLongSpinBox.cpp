/*********************************************************************************
** https://stackoverflow.com/questions/8383620/64bit-int-spin-box-in-qt         **
** modified: Bikbao Rinat Zinorovich                                            **
**********************************************************************************/
#include "QLongLongSpinBox.hpp"
//--------------------------------------------------------------------------------
QLongLongSpinBox::QLongLongSpinBox(QWidget *parent) :
    QAbstractSpinBox(parent)
{
    connect(lineEdit(), &QLineEdit::textEdited, this,   &QLongLongSpinBox::onEditFinished);
};
//--------------------------------------------------------------------------------
QLongLongSpinBox::~QLongLongSpinBox()
{

};
//--------------------------------------------------------------------------------
qlonglong QLongLongSpinBox::value() const
{
    return m_value;
};
//--------------------------------------------------------------------------------
qlonglong QLongLongSpinBox::minimum() const
{
    return m_minimum;
};
//--------------------------------------------------------------------------------
qlonglong QLongLongSpinBox::maximum() const
{
    return m_maximum;
};
//--------------------------------------------------------------------------------
void QLongLongSpinBox::setMinimum(qlonglong min)
{
    m_minimum = min;
}
//--------------------------------------------------------------------------------
void QLongLongSpinBox::setMaximum(qlonglong max)
{
    m_maximum = max;
}
//--------------------------------------------------------------------------------
void QLongLongSpinBox::setRange(qlonglong min, qlonglong max)
{
    setMinimum(min);
    setMaximum(max);
}
//--------------------------------------------------------------------------------
void QLongLongSpinBox::stepBy(int steps)
{
    qlonglong new_value = m_value;
    if (steps < 0 && new_value + steps > new_value)
    {
        new_value = std::numeric_limits<qlonglong>::min();
    }
    else if (steps > 0 && new_value + steps < new_value)
    {
        new_value = std::numeric_limits<qlonglong>::max();
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
QValidator::State QLongLongSpinBox::validate(QString &input, int &pos) const
{
    Q_UNUSED(pos);

    bool ok;
    qlonglong val = input.toLongLong(&ok);
    if (!ok)
        return QValidator::Invalid;

    if (val < m_minimum || val > m_maximum)
        return QValidator::Invalid;

    return QValidator::Acceptable;
}
#else
QValidator::State QLongLongSpinBox::validate(QString &input, int &pos) const
{
    Q_UNUSED(pos);

//    QByteArray temp_ba;
//    temp_ba.append(input);

    bool ok = false;
    qlonglong val = input.toLatin1().toLongLong(&ok, 16);
    //qlonglong val = temp_ba.toLongLong(&ok, 16);
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
qlonglong QLongLongSpinBox::valueFromText(const QString &text) const
{
    bool ok = false;
    return text.toInt(&ok, 16);
}
#else
qlonglong QLongLongSpinBox::valueFromText(const QString &text) const
{
    bool ok = false;
    return text.toLongLong(&ok, 16);
}
#endif
//--------------------------------------------------------------------------------
#ifdef IN_DEC
QString QLongLongSpinBox::textFromValue(qlonglong val) const
{
    return QString::number(val);
}
#else
QString QLongLongSpinBox::textFromValue(qlonglong val) const
{
    return QString("%1").arg(val, 8, 16, QChar('0')).toUpper();
}
#endif
//--------------------------------------------------------------------------------
QAbstractSpinBox::StepEnabled QLongLongSpinBox::stepEnabled() const
{
    return StepUpEnabled | StepDownEnabled;
}
//--------------------------------------------------------------------------------
void QLongLongSpinBox::setValue(qlonglong val)
{
    if (m_value != val)
    {
        lineEdit()->setText(textFromValue(val));
        m_value = val;
    }
}
//--------------------------------------------------------------------------------
void QLongLongSpinBox::onEditFinished()
{
    QString input = lineEdit()->text();
    int pos = 0;
    if (QValidator::Acceptable == validate(input, pos))
        setValue(valueFromText(input));
    else
        lineEdit()->setText(textFromValue(m_value));
}
//--------------------------------------------------------------------------------
