//--------------------------------------------------------------------------------
#ifndef HEXSPINBOX_HPP
#define HEXSPINBOX_HPP
//--------------------------------------------------------------------------------
#include <QSpinBox>
//--------------------------------------------------------------------------------
class QRegExpValidator;
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
    QRegularExpressionValidator *validator;
};
//--------------------------------------------------------------------------------
#endif
