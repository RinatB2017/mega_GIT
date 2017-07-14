/*********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#ifndef MYWIDGET_HPP
#define MYWIDGET_HPP
//--------------------------------------------------------------------------------
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QWidget>
//--------------------------------------------------------------------------------
#define PARAMS_GROUP_NAME   "Params"
//--------------------------------------------------------------------------------
#define MESSAGEBOX_WIDTH    320
//--------------------------------------------------------------------------------
//
//  шаблон для хранения разнообразных данных
//
//--------------------------------------------------------------------------------
template< class T >
class Param
{
public:
    void set(const T& Element)
    {
        value = Element;
    }

    T get(void)
    {
        return value;
    }

private:
    T value;
};
//--------------------------------------------------------------------------------
typedef struct check_box
{
    QString name;
    bool    default_value;
} check_box_t;

typedef struct combo_box
{
    QString name;
    int     default_value;
} combo_box_t;

typedef struct spin_box
{
    QString name;
    int     default_value;
} spin_box_t;

typedef struct d_spin_box
{
    QString name;
    double  default_value;
} d_spin_box_t;
//--------------------------------------------------------------------------------
class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = 0);
    virtual ~MyWidget();

    void connect_log(void);

    void block_this_button(bool state);
    void block_interface(bool state);
    void block_widget(const QString name, bool state);

    bool is_slot_exists(QWidget *obj, const char *slot_sign);

    void load_config(void);
    void save_config(void);

    static bool set_param(QString group_name,QString name, QVariant value);
    static bool get_param(QString group_name,
                          QString name,
                          QVariant default_value,
                          QVariant *value);
    static QStringList get_all_param_name(void);

    void block_close(bool state);

    void load_widgets(QString group_name);
    void save_widgets(QString group_name);

    void load_checkBox(QList<check_box> data);
    void load_comboBox(QList<combo_box> data);
    void load_spinBox(QList<spin_box> data);
    void load_d_spinBox(QList<d_spin_box> data);

    void save_checkBox(QString group_name, QList<QCheckBox *> data);
    void save_comboBox(QString group_name, QList<QComboBox *> data);
    void save_spinBox(QString group_name, QList<QSpinBox *> data);
    void save_d_spinBox(QString group_name, QList<QDoubleSpinBox *> data);

    //---
    int messagebox_noicon(const QString title,
                          const QString text,
                          unsigned int width = MESSAGEBOX_WIDTH);
    int messagebox_info(const QString title,
                        const QString text,
                        unsigned int width = MESSAGEBOX_WIDTH);
    int messagebox_warning(const QString title,
                           const QString text,
                           unsigned int width = MESSAGEBOX_WIDTH);
    int messagebox_critical(const QString title,
                            const QString text,
                            unsigned int width = MESSAGEBOX_WIDTH);
    int messagebox_question(const QString title,
                            const QString text,
                            unsigned int width = MESSAGEBOX_WIDTH);
    //---
    //TODO
    bool is_close = true;

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void message(const QString &);

    void syslog(int level,
                QString src,
                QString message);

public slots:
    bool close(void);

private slots:
    void log(const QString data);

    void debug(void);
    void block_wheel(void);
    void show_objectname(void);

private:
    void load_QCheckBox(QString group_name);
    void load_QComboBox(QString group_name);
    void load_QPushButton(QString group_name);
    void load_QToolButton(QString group_name);
    void load_QTextEdit(QString group_name);
    void load_QLineEdit(QString group_name);
    void load_QSpinBox(QString group_name);
    void load_QDoubleSpinBox(QString group_name);
    void load_QSlider(QString group_name);
    void load_QSplitter(QString group_name);

    void save_QCheckBox(QString group_name);
    void save_QComboBox(QString group_name);
    void save_QPushButton(QString group_name);
    void save_QToolButton(QString group_name);
    void save_QTextEdit(QString group_name);
    void save_QLineEdit(QString group_name);
    void save_QSpinBox(QString group_name);
    void save_QDoubleSpinBox(QString group_name);
    void save_QSlider(QString group_name);
    void save_QSplitter(QString group_name);

protected:
    bool eventFilter(QObject*, QEvent* event);

    void closeEvent(QCloseEvent *);
};
//--------------------------------------------------------------------------------
#endif
