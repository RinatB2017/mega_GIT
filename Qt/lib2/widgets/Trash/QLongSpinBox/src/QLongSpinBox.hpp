/*********************************************************************************
** https://stackoverflow.com/questions/8383620/64bit-int-spin-box-in-qt         **
** modified: Bikbao Rinat Zinorovich                                            **
**********************************************************************************/
#ifndef QLONGGSPINBOX_HPP
#define QLONGSPINBOX_HPP
//--------------------------------------------------------------------------------
#include <QAbstractSpinBox>
#include <QValidator>
#include <QLineEdit>
#include <QDebug>
//--------------------------------------------------------------------------------
class QLongSpinBoxPrivate;
class QLongSpinBox : public QAbstractSpinBox
{
    Q_OBJECT

    Q_PROPERTY(ulong minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(ulong maximum READ maximum WRITE setMaximum)

    Q_PROPERTY(ulong value READ value WRITE setValue NOTIFY valueChanged USER true)

    ulong m_minimum;
    ulong m_maximum;
    ulong m_value;

public:
    explicit QLongSpinBox(QWidget *parent = nullptr);
    ~QLongSpinBox();

    ulong value() const;

    ulong minimum() const;
    ulong maximum() const;

    void setMinimum(ulong min);
    void setMaximum(ulong max);

    void setRange(ulong min, ulong max);

    virtual void stepBy(int steps);

protected:
    //bool event(QEvent *event);
    virtual QValidator::State validate(QString &input, int &pos) const;
    virtual ulong valueFromText(const QString &text) const;
    virtual QString textFromValue(ulong val) const;
    //virtual void fixup(QString &str) const;
    virtual QAbstractSpinBox::StepEnabled stepEnabled() const;

public slots:
    void setValue(ulong val);
    void onEditFinished();

Q_SIGNALS:
    void valueChanged(ulong v);

private:
    Q_DISABLE_COPY(QLongSpinBox)
    Q_DECLARE_PRIVATE(QLongSpinBox)
};
//--------------------------------------------------------------------------------
#endif
