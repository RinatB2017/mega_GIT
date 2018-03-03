/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
**                                                                              **
**     This program is free software: you can redistribute it and/or modify     **
**     it under the terms of the GNU General Public License as published by     **
**     the Free Software Foundation, either version 3 of the License, or        **
**     (at your option) any later version.                                      **
**                                                                              **
**     This program is distributed in the hope that it will be useful,          **
**     but WITHOUT ANY WARRANTY; without even the implied warranty of           **
**     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **
**     GNU General Public License for more details.                             **
**                                                                              **
**     You should have received a copy of the GNU General Public License        **
**     along with this program.  If not, see http://www.gnu.org/licenses/.      **
**                                                                              **
**********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#ifndef MYWIDGET_HPP
#define MYWIDGET_HPP
//--------------------------------------------------------------------------------
#include <QFileDialog>
#include <QDateTime>
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
class QToolButton;
class QToolBar;
//--------------------------------------------------------------------------------
class MyWidget : public QWidget
{
    Q_OBJECT
public:
    MyWidget(QWidget *parent = 0);
    ~MyWidget();

    void init_w_lists(void);
    void lock_interface(void);
    void unlock_interface(void);

    void lock_this_button(void);
    void unlock_this_button(void);

    void block_this_button(bool state);
    void block_interface(bool state);
    void block_widget(const QString name,
                      bool state);

    bool is_slot_exists(QWidget *obj,
                        const char *slot_sign);

    void load_config(void);
    void save_config(void);

    static bool set_param(QString group_name,
                          QString name,
                          QVariant value);
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
    static int messagebox_noicon(const QString title,
                                 const QString text,
                                 unsigned int width = MESSAGEBOX_WIDTH);
    static int messagebox_info(const QString title,
                               const QString text,
                               unsigned int width = MESSAGEBOX_WIDTH);
    static int messagebox_warning(const QString title,
                                  const QString text,
                                  unsigned int width = MESSAGEBOX_WIDTH);
    static int messagebox_critical(const QString title,
                                   const QString text,
                                   unsigned int width = MESSAGEBOX_WIDTH);
    static int messagebox_question(const QString title,
                                   const QString text,
                                   unsigned int width = MESSAGEBOX_WIDTH);

    bool create_pixmap(QWidget *w_left, QWidget *w_central);

    QToolButton *add_button(QToolBar *tool_bar,
                            QToolButton *tool_button,
                            QIcon icon,
                            const QString &text,
                            const QString &tool_tip);

    //---
    bool is_close = true;

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void colorLog(const QString &, const QColor, const QColor);

    void syslog(int level,
                QString src,
                QString message);
    void syslog(QDateTime dt,
                int level,
                int src,
                QString message);

public slots:
    bool close(void);

private slots:
    void log(const QString data);

    void debug(void);
    void block_wheel(void);
    void show_objectname(void);

private:
    void connect_log(QWidget *parent);

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

    QList<QWidget *> w_lists;

protected:
    virtual void updateText(void) = 0;
    void changeEvent(QEvent *event);
#ifndef BLOCK_WHEEL
    bool eventFilter(QObject*, QEvent* event);
#endif
    void closeEvent(QCloseEvent *);
};
//--------------------------------------------------------------------------------
#endif
