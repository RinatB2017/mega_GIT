//--------------------------------------------------------------------------------
#ifndef LCD_KNOB_H
#define LCD_KNOB_H
//--------------------------------------------------------------------------------
#include <QPointer>
#include <QFrame>
//--------------------------------------------------------------------------------
class QwtKnob;
class QLCDNumber;
class QVBoxLayout;
//--------------------------------------------------------------------------------
class LCD_Knob : public QFrame
{
    Q_OBJECT
    
public:
    explicit LCD_Knob(QWidget *parent = nullptr);
    ~LCD_Knob();

    void setRange(double, double);
    void setScaleMaxMajor(int);
    void setScaleMaxMinor(int);
    void setValue(double);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void valueChanged(double);

private:
    QPointer<QwtKnob>     knob;
    QPointer<QLCDNumber>  lcd;
    QPointer<QVBoxLayout> vbox;
};
//--------------------------------------------------------------------------------
#endif // LCD_KNOB_H
