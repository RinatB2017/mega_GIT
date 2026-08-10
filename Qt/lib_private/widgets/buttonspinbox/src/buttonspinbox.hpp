//--------------------------------------------------------------------------------
#ifndef _BUTTONSPINBOX_H
#define _BUTTONSPINBOX_H
//--------------------------------------------------------------------------------
#include <QFrame>
//--------------------------------------------------------------------------------
class HexSpinBox;
class QPushButton;
class QHBoxLayout;
//--------------------------------------------------------------------------------
class ButtonSpinBox : public QFrame
{
    Q_OBJECT
private:
    QHBoxLayout *hbox;
    QPushButton *button;
    HexSpinBox *spinbox;

public slots:
    void enable(bool state);

public:
    ButtonSpinBox(QWidget* parent = 0);
    void init(const QString &text, int min, int max, int value);
    int set_name(const QString &text_button);
    int set_value(int value);
    int get_value(void);

protected:

signals:
    void value_changed();
};
//--------------------------------------------------------------------------------
#endif
