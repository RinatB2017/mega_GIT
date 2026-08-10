//--------------------------------------------------------------------------------
#ifndef HEXSPINBOX_HPP
#define HEXSPINBOX_HPP
//--------------------------------------------------------------------------------
#include <QSpinBox>
//--------------------------------------------------------------------------------
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#   include <QRegExpValidator>
#else
#   include <QRegularExpressionValidator>
#endif
//--------------------------------------------------------------------------------
class HexSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    HexSpinBox(QWidget *parent = nullptr);

protected:
    QValidator::State validate(QString &text, int &pos) const;
    int valueFromText(const QString &text) const;
    QString textFromValue(int value) const;

private:
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QRegExpValidator *validator;
#else
    QRegularExpressionValidator *validator;
#endif
};
//--------------------------------------------------------------------------------
#endif
