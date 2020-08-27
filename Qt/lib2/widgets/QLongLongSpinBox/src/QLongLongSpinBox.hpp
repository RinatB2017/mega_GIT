/*********************************************************************************
** https://stackoverflow.com/questions/8383620/64bit-int-spin-box-in-qt         **
** modified: Bikbao Rinat Zinorovich                                            **
**********************************************************************************/
#ifndef QLONGLONGSPINBOX_HPP
#define QLONGLONGSPINBOX_HPP
//--------------------------------------------------------------------------------
#include <QAbstractSpinBox>
#include <QValidator>
#include <QLineEdit>
#include <QDebug>
//--------------------------------------------------------------------------------
class QLongLongSpinBoxPrivate;
class QLongLongSpinBox : public QAbstractSpinBox
{
    Q_OBJECT

    Q_PROPERTY(qlonglong minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(qlonglong maximum READ maximum WRITE setMaximum)

    Q_PROPERTY(qlonglong value READ value WRITE setValue NOTIFY valueChanged USER true)

    qlonglong m_minimum;
    qlonglong m_maximum;
    qlonglong m_value;

public:
    explicit QLongLongSpinBox(QWidget *parent = nullptr);
    ~QLongLongSpinBox();

    qlonglong value() const;

    qlonglong minimum() const;
    qlonglong maximum() const;

    void setMinimum(qlonglong min);
    void setMaximum(qlonglong max);

    void setRange(qlonglong min, qlonglong max);

    virtual void stepBy(int steps);

protected:
    //bool event(QEvent *event);
    virtual QValidator::State validate(QString &input, int &pos) const;
    virtual qlonglong valueFromText(const QString &text) const;
    virtual QString textFromValue(qlonglong val) const;
    //virtual void fixup(QString &str) const;
    virtual QAbstractSpinBox::StepEnabled stepEnabled() const;

public slots:
    void setValue(qlonglong val);
    void onEditFinished();

Q_SIGNALS:
    void valueChanged(qlonglong v);

private:
    Q_DISABLE_COPY(QLongLongSpinBox)
    Q_DECLARE_PRIVATE(QLongLongSpinBox)
};
//--------------------------------------------------------------------------------
#endif
