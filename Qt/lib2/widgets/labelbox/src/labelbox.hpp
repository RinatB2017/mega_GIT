//--------------------------------------------------------------------------------
#ifndef _LABELBOX_H
#define _LABELBOX_H
//--------------------------------------------------------------------------------
#include <QFrame>
//--------------------------------------------------------------------------------
class QHBoxLayout;
class QLabel;
//--------------------------------------------------------------------------------
class LabelBox : public QFrame
{
    Q_OBJECT
    private:
        QString name;
        QHBoxLayout* hbox;
        QLabel* label;
        int state;

    public slots:

    public:
        LabelBox(const QString &name_label,
                 QWidget* parent = 0);
        int set_name(const QString &text);
        void set_state(int state);
        int get_state(void);

    protected:

    signals:
      
};
//--------------------------------------------------------------------------------
#endif
