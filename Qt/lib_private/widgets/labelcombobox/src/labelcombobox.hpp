//--------------------------------------------------------------------------------
#ifndef _LABELCOMBOBOX_H
#define _LABELCOMBOBOX_H
//--------------------------------------------------------------------------------
#include <QString>
#include <QFrame>
//--------------------------------------------------------------------------------
class QHBoxLayout;
class QLabel;
class QComboBox;
class QWidget;
//--------------------------------------------------------------------------------
class LabelComboBox : public QFrame
{
    Q_OBJECT
    private:
        QHBoxLayout *hbox;
        QLabel *label;
        QComboBox *combobox;
        QString n_frame;
        QString n_label;
        int init_index;
        bool flag_log;

    public slots:
        void change_state(void);
        void enable(bool state);

    signals:
        void click_state(void);

        void log(const QString &);

    public:
        LabelComboBox(const QString &name_frame,
                      const QString &name_label,
                      const QStringList &sl_combobox,
                      int index = 0,
                      int maxw = 300,
                      QWidget* parent = 0);
        void init(void);
        void set_name(const QString &name);
        QString get_name(void);
        int set_index(int index);
        unsigned char get_index(void);
        QString get_text(void);

    protected:

};
//--------------------------------------------------------------------------------
#endif
