//--------------------------------------------------------------------------------
#ifndef LCD_KNOB_H
#define LCD_KNOB_H
//--------------------------------------------------------------------------------
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
    explicit LCD_Knob(QWidget *parent = 0);
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
    QwtKnob *knob = 0;
    QLCDNumber *lcd = 0;
    QVBoxLayout *vbox = 0;

};
//--------------------------------------------------------------------------------
#endif // LCD_KNOB_H
