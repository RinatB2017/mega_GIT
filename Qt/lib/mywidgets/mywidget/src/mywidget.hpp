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
#include <QDoubleSpinBox>
#include <QRadioButton>
#include <QStringList>
#include <QToolButton>
#include <QCheckBox>
#include <QComboBox>
#include <QFileInfo>
#include <QLineEdit>
#include <QToolBar>
#include <QPainter>
#include <QPointer>
#include <QWidget>
#include <QDir>
//--------------------------------------------------------------------------------
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#   include <QTextCodec>
#   include <QRegExp>
#endif
//--------------------------------------------------------------------------------
#include "digital_clock.hpp"
#include "lcd_clock.hpp"
#include "mymessages.hpp"
#include "mysettings.hpp"
#include "imywidget.hpp"
//--------------------------------------------------------------------------------
//
//  шаблон для хранения разнообразных данных
//
//--------------------------------------------------------------------------------
template< class T >
class Param
{
public:
    void set(const T& Element) { value = Element; }
    T get(void) { return value; }

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
class MyWidget : public QWidget, public MyMessages, public MySettings, public iMyWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = nullptr);
    virtual ~MyWidget();

    void init_w_lists(void);
    void add_widget_to_w_lists(QWidget *widget);
    void lock_interface(void);
    void unlock_interface(void);

    void lock_this_button(void);
    void unlock_this_button(void);

    void block_this_button(bool state);
    void block_widget(const QString &name,
                      bool state);

    bool is_slot_exists(QWidget *obj,
                        const char *slot_sign);

    bool is_letter_or_number(QByteArray ba);

    static bool set_param(QString group_name,
                          QString name,
                          QVariant value);
    static bool get_param(QString group_name,
                          QString name,
                          QVariant default_value,
                          QVariant *value);
    static QStringList get_all_param_name(void);

    void block_close(bool state);

    void load_widgets(void);
    void save_widgets(void);

    void load_checkBox(QList<check_box> data);
    void load_comboBox(QList<combo_box> data);
    void load_spinBox(QList<spin_box> data);
    void load_d_spinBox(QList<d_spin_box> data);

    //---
    bool create_pixmap(QWidget *w_left, QWidget *w_central);

    QToolButton *add_button(QToolBar *tool_bar,
                            QToolButton *tool_button,
                            QIcon icon,
                            const QString &text,
                            const QString &tool_tip);

    bool connect_log_signals(QWidget *src, QWidget *dest);

    bool fail(const QString &text = "Пока не сделано");

    void check_file(const QString &orig_file, const QString &copy_file);

    bool add_digital_clock(bool add_spacer = true);
    bool add_lcd_clock(bool add_spacer = true);

    QString get_class_name(const QString &fullname);
    QString get_func_name(const QString &fullname);

    void set_all_buttons_no_save(void);
    void show_objectNames(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void clear_log(void);

    void colorLog(const QString &, const QColor, const QColor);

public slots:
    void block_interface(bool state);
    void check_tooltips(void);
#ifdef BLOCK_WHEEL
    void block_wheel(void);
#endif

private slots:
    void log(const QString &data);

    void s_test(void);
    void show_objectname(void);

private:
    void connect_log(QWidget *parent);
    bool check_exists_signals(QWidget *parent);

    // templates
    template<typename T>
    void lock_widget(bool state);

    template<typename T>
    void set_property_widget(void);
    //---

    QList<QWidget *> w_lists;
    QStringList sl_properties_of_widgets;

protected:
    void changeEvent(QEvent *event);
#ifdef BLOCK_WHEEL
    bool eventFilter(QObject*, QEvent* event);
#endif
    virtual void closeEvent(QCloseEvent *);
};
//--------------------------------------------------------------------------------
#endif
