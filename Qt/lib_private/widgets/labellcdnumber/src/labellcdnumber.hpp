//--------------------------------------------------------------------------------
#ifndef _LABELLCDNUMBER_H
#define _LABELLCDNUMBER_H
//--------------------------------------------------------------------------------
#include <QString>
#include <QFrame>
//--------------------------------------------------------------------------------
class QHBoxLayout;
class QLabel;
class QLCDNumber;
class QWidget;
//--------------------------------------------------------------------------------
class LabelLCDNumber : public QFrame
{
    Q_OBJECT
    private:
        QString name;
        QHBoxLayout *hbox;
        QLabel *label;
        QLCDNumber *lcdnumber;

    public slots:
        void over(void);

    public:
        LabelLCDNumber(const QString &name_label,
                       int nDigits,
                       int value,
                       QWidget* parent = 0);
        void set_value(int value);
        int get_value(void);

    protected:

    signals:
        void overflow();

        void log(const QString &);
};
//--------------------------------------------------------------------------------
#endif
