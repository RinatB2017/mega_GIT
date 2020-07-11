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
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "mywidget.hpp"
#include "logbox.hpp"
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    MySettings()
{
#ifndef RS232_LOG
    //TODO не надо тут условий. Родитель может быть и пустым, не надо приводить всё к toplevelwidget
    connect_log(parent);
#endif
#ifdef QT_DEBUG
    qDebug() << "MyWidget()";
    QTimer::singleShot(100, this, SLOT(s_test()));
#endif

    setAttribute(Qt::WA_DeleteOnClose);
}
//--------------------------------------------------------------------------------
MyWidget::~MyWidget()
{
#ifdef QT_DEBUG
    QString on = objectName();
    if(on.isEmpty())
    {
        on = "no name";
    }
    qDebug() << "~MyWidget()" << on;
    //qDebug() << "~MyWidget()" << objectName();
#endif
}
//--------------------------------------------------------------------------------
bool MyWidget::check_exists_signals(QWidget *parent)
{
    if(parent == nullptr)
    {
        return false;
    }
    int m_info  = parent->metaObject()->indexOfSignal("info(QString)");
    int m_debug = parent->metaObject()->indexOfSignal("debug(QString)");
    int m_error = parent->metaObject()->indexOfSignal("error(QString)");
    int m_trace = parent->metaObject()->indexOfSignal("trace(QString)");
    int m_colorlog = parent->metaObject()->indexOfSignal("colorLog(QString,QColor,QColor)");

    if(m_info == -1)    return false;
    if(m_debug == -1)   return false;
    if(m_error == -1)   return false;
    if(m_trace == -1)   return false;
    if(m_colorlog == -1)   return false;

    connect(this,   SIGNAL(info(QString)),  parent, SIGNAL(info(QString)));
    connect(this,   SIGNAL(debug(QString)), parent, SIGNAL(debug(QString)));
    connect(this,   SIGNAL(error(QString)), parent, SIGNAL(error(QString)));
    connect(this,   SIGNAL(trace(QString)), parent, SIGNAL(trace(QString)));

    connect(this,   SIGNAL(clear_log()),    parent, SIGNAL(clear_log()));

    connect(this,   SIGNAL(colorLog(QString, QColor, QColor)),  parent, SIGNAL(colorLog(QString, QColor, QColor)));

    return true;
}
//--------------------------------------------------------------------------------
// основная функция
void MyWidget::connect_log(QWidget *parent)
{
#ifdef NO_LOG
    //    connect(this, SIGNAL(info(QString)),    this, SLOT(log(QString)));
    //    connect(this, SIGNAL(debug(QString)),   this, SLOT(log(QString)));
    //    connect(this, SIGNAL(error(QString)),   this, SLOT(log(QString)));
    //    connect(this, SIGNAL(trace(QString)),   this, SLOT(log(QString)));

    int m_info  = parent->metaObject()->indexOfSignal("info(QString)");
    int m_debug = parent->metaObject()->indexOfSignal("debug(QString)");
    int m_error = parent->metaObject()->indexOfSignal("error(QString)");
    int m_trace = parent->metaObject()->indexOfSignal("trace(QString)");

    if(m_info != -1)    connect(this, SIGNAL(info(QString)),    parent, SIGNAL(info(QString)));
    if(m_debug != -1)   connect(this, SIGNAL(debug(QString)),   parent, SIGNAL(debug(QString)));
    if(m_error != -1)   connect(this, SIGNAL(error(QString)),   parent, SIGNAL(error(QString)));
    if(m_trace != -1)   connect(this, SIGNAL(trace(QString)),   parent, SIGNAL(trace(QString)));

    Q_UNUSED(parent)
#else
    if(parent)
    {
        if(!check_exists_signals(parent))
        {
            if(!check_exists_signals(topLevelWidget()))
            {
                connect(this, SIGNAL(info(QString)),    this, SLOT(log(QString)));
                connect(this, SIGNAL(debug(QString)),   this, SLOT(log(QString)));
                connect(this, SIGNAL(error(QString)),   this, SLOT(log(QString)));
                connect(this, SIGNAL(trace(QString)),   this, SLOT(log(QString)));

                connect(this,   SIGNAL(colorLog(QString, QColor, QColor)),   this, SLOT(log(QString)));
            }
        }
    }
    else
    {
        connect(this,   SIGNAL(info(QString)),      this,   SLOT(log(QString)));
        connect(this,   SIGNAL(debug(QString)),     this,   SLOT(log(QString)));
        connect(this,   SIGNAL(error(QString)),     this,   SLOT(log(QString)));
        connect(this,   SIGNAL(trace(QString)),     this,   SLOT(log(QString)));

        connect(this,   SIGNAL(colorLog(QString,    QColor, QColor)),   this, SLOT(log(QString)));
    }
#endif
}
//--------------------------------------------------------------------------------
bool MyWidget::connect_log_signals(QWidget *src, QWidget *dest)
{
    Q_CHECK_PTR(src);
    Q_CHECK_PTR(dest);

    if(src == nullptr)  return false;
    if(dest == nullptr)  return false;

    Q_ASSERT(src->metaObject()->indexOfSignal("info(QString)")  != -1);
    Q_ASSERT(src->metaObject()->indexOfSignal("debug(QString)") != -1);
    Q_ASSERT(src->metaObject()->indexOfSignal("error(QString)") != -1);
    Q_ASSERT(src->metaObject()->indexOfSignal("trace(QString)") != -1);

    Q_ASSERT(dest->metaObject()->indexOfSignal("info(QString)")  != -1);
    Q_ASSERT(dest->metaObject()->indexOfSignal("debug(QString)") != -1);
    Q_ASSERT(dest->metaObject()->indexOfSignal("error(QString)") != -1);
    Q_ASSERT(dest->metaObject()->indexOfSignal("trace(QString)") != -1);

    //TODO надо переделать на современный стиль
    connect(src,    SIGNAL(info(QString)),  dest,   SIGNAL(info(QString)));
    connect(src,    SIGNAL(debug(QString)), dest,   SIGNAL(debug(QString)));
    connect(src,    SIGNAL(error(QString)), dest,   SIGNAL(error(QString)));
    connect(src,    SIGNAL(trace(QString)), dest,   SIGNAL(trace(QString)));

    return true;
}
//--------------------------------------------------------------------------------
void MyWidget::log(const QString data)
{
#if 1
    qDebug() << data;
#else
#ifdef QT_DEBUG
    qDebug() << data;
#else
    Q_UNUSED(data)
#endif
#endif
}
//--------------------------------------------------------------------------------
void MyWidget::init_w_lists(void)
{
    QList<QLineEdit *> allle = findChildren<QLineEdit *>();
    foreach (QLineEdit *obj, allle)
    {
        obj->setProperty("state", true);
        w_lists.append(obj);
    }
    QList<QAbstractSlider *> allsl = findChildren<QAbstractSlider *>();
    foreach (QAbstractSlider *obj, allsl)
    {
        obj->setProperty("state", true);
        w_lists.append(obj);
    }
    QList<QAbstractScrollArea *> allsa = findChildren<QAbstractScrollArea *>();
    foreach (QAbstractScrollArea *obj, allsa)
    {
        obj->setProperty("state", true);
        w_lists.append(obj);
    }
    QList<QAbstractSpinBox *> allsb = findChildren<QAbstractSpinBox *>();
    foreach (QAbstractSpinBox *obj, allsb)
    {
        obj->setProperty("state", true);
        w_lists.append(obj);
    }
    QList<QComboBox *> allcb = findChildren<QComboBox *>();
    foreach (QComboBox *obj, allcb)
    {
        obj->setProperty("state", true);
        w_lists.append(obj);
    }
    QList<QAbstractButton *> allbtn = findChildren<QAbstractButton *>();
    foreach (QAbstractButton *obj, allbtn)
    {
        obj->setProperty("state", true);
        w_lists.append(obj);
    }
}
//--------------------------------------------------------------------------------
void MyWidget::add_widget_to_w_lists(QWidget *widget)
{
    Q_CHECK_PTR(widget);
    w_lists.append(widget);
}
//--------------------------------------------------------------------------------
void MyWidget::lock_interface(void)
{
    foreach (QWidget *btn, w_lists)
    {
        btn->setDisabled(true);
    }
}
//--------------------------------------------------------------------------------
void MyWidget::unlock_interface(void)
{
    foreach (QWidget *btn, w_lists)
    {
        btn->setEnabled(true);
    }
}
//--------------------------------------------------------------------------------
void MyWidget::lock_this_button(void)
{
    if (QAbstractButton *pb=qobject_cast<QAbstractButton *>(sender()))
    {
        pb->setDisabled(true);
    }
}
//--------------------------------------------------------------------------------
void MyWidget::unlock_this_button(void)
{
    if (QAbstractButton *pb=qobject_cast<QAbstractButton *>(sender()))
    {
        pb->setEnabled(true);
    }
}
//--------------------------------------------------------------------------------
void MyWidget::block_this_button(bool state)
{
    if (QAbstractButton *pb=qobject_cast<QAbstractButton *>(sender()))
    {
        pb->setDisabled(state);
    }
}
//--------------------------------------------------------------------------------
void MyWidget::block_interface(bool state)
{
    QList<QAbstractButton *> all_obj = findChildren<QAbstractButton *>();
    foreach(QAbstractButton *obj, all_obj)
    {
        if(obj->property(NO_BLOCK).toBool() == false)
        {
            obj->setDisabled(state);
        }
    }
    //---
    QList<QComboBox *> all_cb = findChildren<QComboBox *>();
    foreach(QComboBox *obj, all_cb)
    {
        if(obj->property(NO_BLOCK).toBool() == false)
        {
            obj->setDisabled(state);
        }
    }
    //---
    QList<QDoubleSpinBox *> all_dsb = findChildren<QDoubleSpinBox *>();
    foreach(QDoubleSpinBox *obj, all_dsb)
    {
        if(obj->property(NO_BLOCK).toBool() == false)
        {
            obj->setDisabled(state);
        }
    }
    //---
    QList<QSpinBox *> all_sb = findChildren<QSpinBox *>();
    foreach(QSpinBox *obj, all_sb)
    {
        if(obj->property(NO_BLOCK).toBool() == false)
        {
            obj->setDisabled(state);
        }
    }
}
//--------------------------------------------------------------------------------
void MyWidget::block_widget(const QString name, bool state)
{
    if(name.isEmpty())
    {
        return;
    }
    QList<QWidget *> all_obj = findChildren<QWidget *>();
    foreach(QWidget *obj, all_obj)
    {
        if(obj->objectName() == name)
        {
            if(obj->property(NO_BLOCK).toBool() == false)
            {
                obj->setDisabled(state);
            }
        }
    }
}
//--------------------------------------------------------------------------------
bool MyWidget::is_slot_exists(QWidget *obj, const char *slot_sign)
{
    Q_CHECK_PTR(obj);
    Q_CHECK_PTR(slot_sign);

    if(obj->metaObject()->indexOfSlot(QMetaObject::normalizedSignature(qPrintable(slot_sign))) == -1) return false;
    return true;
}
//--------------------------------------------------------------------------------
bool MyWidget::set_param(QString group_name, QString name, QVariant value)
{
    if(name.isEmpty())  return false;

    QString org_name = ORGNAME;
#ifdef QT_DEBUG
    QString app_name = QString("%1(debug)").arg(APPNAME);
#else
    QString app_name = APPNAME;
#endif

#ifndef SAVE_INI
    QSettings *settings = new QSettings(org_name, app_name);
#else
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
#endif
    Q_CHECK_PTR(settings);

    settings->beginGroup(group_name);
    settings->setValue(name, value);
    settings->endGroup();

    settings->deleteLater();
    return true;
}
//--------------------------------------------------------------------------------
bool MyWidget::get_param(QString group_name,
                         QString name,
                         QVariant default_value,
                         QVariant *value)
{
    if(name.isEmpty())
    {
        return false;
    }

    QString org_name = ORGNAME;
#ifdef QT_DEBUG
    QString app_name = QString("%1(debug)").arg(APPNAME);
#else
    QString app_name = APPNAME;
#endif

#ifndef SAVE_INI
    QSettings *settings = new QSettings(org_name, app_name);
#else
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
#endif
    Q_CHECK_PTR(settings);

    settings->beginGroup(group_name);
    QVariant temp = settings->value(name, default_value);
    settings->endGroup();

    settings->deleteLater();
    *value = temp;
    return true;
}
//--------------------------------------------------------------------------------
QStringList MyWidget::get_all_param_name(void)
{
    QString org_name = ORGNAME;
#ifdef QT_DEBUG
    QString app_name = QString("%1(debug)").arg(APPNAME);
#else
    QString app_name = APPNAME;
#endif

#ifndef SAVE_INI
    QSettings *settings = new QSettings(org_name, app_name);
#else
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
#endif
    Q_CHECK_PTR(settings);

    settings->beginGroup(PARAMS_GROUP_NAME);
    QStringList temp = settings->allKeys();
    settings->endGroup();

    settings->deleteLater();
    return temp;
}
//--------------------------------------------------------------------------------
QString MyWidget::get_full_objectName(QWidget *widget)
{
    Q_CHECK_PTR(widget);

    QStringList sl;
    QWidget *temp = static_cast<QWidget *>(widget);
    do {
        if(temp)
        {
            sl.append(temp->objectName());
        }
        temp = static_cast<QWidget *>(temp->parent());
    } while(temp);

    QString temp_string;
    for(int n=sl.count(); n>0; n--)
    {
        QString str = sl.at(n-1);
        if(str.isEmpty() == false)
        {
            temp_string.append(QString("%1/").arg(str));
        }
    }
    temp_string = temp_string.remove(temp_string.length()-1, 1);
    if(temp_string.isEmpty()) temp_string = "xxx";
    return temp_string;
}
//--------------------------------------------------------------------------------
void MyWidget::block_close(bool state)
{
    setProperty("flag_no_close", state);
}
//--------------------------------------------------------------------------------
void MyWidget::closeEvent(QCloseEvent *)
{
#ifdef QT_DEBUG
    qDebug() << "closeEvent";
#endif
}
//--------------------------------------------------------------------------------
void MyWidget::s_test(void)
{
    show_objectname();
    // block_wheel();
}
//--------------------------------------------------------------------------------
#ifndef BLOCK_WHEEL
void MyWidget::block_wheel(void)
{
    QList<QWidget *> all_obj = findChildren<QWidget *>();
    foreach(QWidget *obj, all_obj)
    {
        if(obj->objectName().isEmpty() == false)
        {
            obj->installEventFilter(this);
        }
    }
}
#endif
//--------------------------------------------------------------------------------
void MyWidget::show_objectname(void)
{
    QList<QWidget *> all_obj = findChildren<QWidget *>();
    foreach(QWidget *obj, all_obj)
    {
        if(obj->objectName().isEmpty() == false)
        {
            if(obj->objectName().left(3) != "qt_")
            {
                obj->setToolTip(obj->objectName());
            }
        }
    }
}
//--------------------------------------------------------------------------------
void MyWidget::load_checkBox(QList<check_box> data)
{
    QList<QCheckBox *> all_chb = findChildren<QCheckBox *>();

    foreach (check_box cb_obj, data) {
        foreach(QCheckBox *obj, all_chb)
        {
            if(obj->objectName() == cb_obj.name)
            {
                obj->setChecked(cb_obj.default_value);
            }
        }
    }
}
//--------------------------------------------------------------------------------
void MyWidget::load_comboBox(QList<combo_box> data)
{
    QList<QComboBox *> all_chb = findChildren<QComboBox *>();

    foreach (combo_box cb_obj, data) {
        foreach(QComboBox *obj, all_chb)
        {
            if(obj->objectName() == cb_obj.name)
            {
                obj->setCurrentIndex(cb_obj.default_value);
            }
        }
    }
}
//--------------------------------------------------------------------------------
void MyWidget::load_spinBox(QList<spin_box> data)
{
    QList<QSpinBox *> all_chb = findChildren<QSpinBox *>();

    foreach (spin_box cb_obj, data) {
        foreach(QSpinBox *obj, all_chb)
        {
            if(obj->objectName() == cb_obj.name)
            {
                obj->setValue(cb_obj.default_value);
            }
        }
    }
}
//--------------------------------------------------------------------------------
void MyWidget::load_d_spinBox(QList<d_spin_box> data)
{
    QList<QDoubleSpinBox *> all_chb = findChildren<QDoubleSpinBox *>();

    foreach (d_spin_box cb_obj, data) {
        foreach(QDoubleSpinBox *obj, all_chb)
        {
            if(obj->objectName() == cb_obj.name)
            {
                obj->setValue(cb_obj.default_value);
            }
        }
    }
}
//--------------------------------------------------------------------------------
bool MyWidget::create_pixmap(QWidget *w_left, QWidget *w_central)
{
    Q_CHECK_PTR(w_left);
    Q_CHECK_PTR(w_central);

    QStringList *sl = reinterpret_cast<QStringList *>(w_left);
    if(sl)
    {
        QString text;
        foreach (QString txt, *sl)
        {
            text.append(txt);
        }
        //---
        QWidget *frame = new QWidget;
        QTextEdit *te = new QTextEdit;
        QFont font("Courier", 10);
        te->setFont(font);
        te->setReadOnly(true);
        te->setPlainText(text);
        QVBoxLayout *vbox = new QVBoxLayout();
        vbox->setMargin(0);
        vbox->addWidget(te);
        frame->setLayout(vbox);
        frame->setFixedSize(te->width(), w_central->height());
        //---
#ifdef HAVE_QT5
        QPixmap p_central = w_central->grab(QRect(0, 0, w_central->width(), w_central->height()));
        QPixmap left_pixmap = (static_cast<QWidget *>(frame)->grab(QRect(0, 0, frame->width(), frame->height())));
        QPixmap *main_pixmap = new QPixmap(left_pixmap.width() + p_central.width(), p_central.height());
        //---
        QPainter painter;
        painter.begin(main_pixmap);
        painter.drawPixmap(0, 0, left_pixmap);
        painter.drawPixmap(left_pixmap.width(), 0, p_central);
        painter.end();
        //---

        QLabel *label = new QLabel;
        label->setPixmap(*main_pixmap);
        label->show();

        frame->deleteLater();
#endif
    }

    return true;
}
//--------------------------------------------------------------------------------
QToolButton *MyWidget::add_button(QToolBar *tool_bar,
                                  QToolButton *tool_button,
                                  QIcon icon,
                                  const QString &text,
                                  const QString &tool_tip)
{
    Q_CHECK_PTR(tool_bar);
    Q_CHECK_PTR(tool_button);

    tool_button->setIcon(icon);
    tool_button->setText(QObject::tr(text.toLocal8Bit()));
    tool_button->setToolTip(QObject::tr(tool_tip.toLocal8Bit()));
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void MyWidget::check_tooltips(void)
{
    MainWindow *mw = reinterpret_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    QList<QAbstractButton *> l_obj = mw->findChildren<QAbstractButton *>();
    foreach (QAbstractButton *obj, l_obj)
    {
        if(obj->objectName().left(3) == "qt_")
        {
            continue;
        }
        if(obj->objectName().left(3) == "tb_")
        {
            continue;
        }
        if(obj->toolTip().isEmpty())
        {
            emit error(QString("obj [%1] [%2] no tooltip")
                       .arg(obj->objectName())
                       .arg(obj->text()));
        }
    }
}
//--------------------------------------------------------------------------------
void MyWidget::fail(void)
{
    emit error("Пока не сделано");
}
//--------------------------------------------------------------------------------
void MyWidget::changeEvent(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::LanguageChange:
        updateText();
        break;

    default:
        QWidget::changeEvent(event);
        break;
    }
}
//--------------------------------------------------------------------------------
#ifndef BLOCK_WHEEL
bool MyWidget::eventFilter(QObject*, QEvent* event)
{
#if 0
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    //---
    if(mouseEvent->type() == QMouseEvent::Wheel)
    {
        return true;
    }
    return false;
#else
    if(event->type() == QEvent::Wheel)
    {
        qDebug() << "Wheel event blocked";
        return true;
    }
    return false;
#endif
}
#endif
//--------------------------------------------------------------------------------
void MyWidget::load_widgets(void)
{
    MainWindow *mw = reinterpret_cast<MainWindow *>(topLevelWidget());
    if(mw)
    {
        mw->load_setting();
    }

#ifdef USE_TOPLEVELWIDGETS
    QList<QWidget *> widgets = topLevelWidget()->findChildren<QWidget *>();
#else
    QList<QWidget *> widgets = findChildren<QWidget *>();
#endif

    Q_ASSERT(widgets.count() != 0);

    foreach(QWidget *widget, widgets)
    {
        if(is_my_widget(widget->objectName()))
        {
            if(widget->property(NO_SAVE).toBool() == true)
            {
                continue;
            }

            beginGroup(get_full_objectName(widget));
            load_combobox_property(widget);
            load_splitter_property(widget);
            load_property(widget, "isEnabled");
            load_property(widget, "checked");
            load_property(widget, "text");
            load_property(widget, "value");
            load_property(widget, "position");
            load_property(widget, "state");
            load_property(widget, "time");
            load_property(widget, "date");
            load_property(widget, "plainText");
            endGroup();
        }
    }
}
//--------------------------------------------------------------------------------
void MyWidget::save_widgets(void)
{
//    не надо это раскомментировать
//    MainWindow *mw = reinterpret_cast<MainWindow *>(topLevelWidget());
//    if(mw)
//    {
//        mw->save_setting();
//    }

#ifdef USE_TOPLEVELWIDGETS
    QList<QWidget *> widgets = topLevelWidget()->findChildren<QWidget *>();
#else
    QList<QWidget *> widgets = findChildren<QWidget *>();
#endif

    //Q_ASSERT(widgets.count() != 0);

    foreach(QWidget *widget, widgets)
    {
        Q_CHECK_PTR(widget);
        if(is_my_widget(widget->objectName()))
        {
            if(widget->property(NO_SAVE).toBool() == true)
            {
                continue;
            }

            beginGroup(get_full_objectName(widget));
            save_combobox_property(widget);
            save_splitter_property(widget);
            save_property(widget, "isEnabled");
            save_property(widget, "checked");
            save_property(widget, "text");
            save_property(widget, "value");
            save_property(widget, "position");
            save_property(widget, "state");
            save_property(widget, "time");
            save_property(widget, "date");
            save_property(widget, "plainText");
            endGroup();
        }
    }
}
//--------------------------------------------------------------------------------
