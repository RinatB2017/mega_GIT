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
#include "mainwindow.hpp"
#include "mywidget.hpp"
#include "logbox.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
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
    //qDebug() << "MyWidget()";
    //QTimer::singleShot(0, this, SLOT(s_test()));
#endif

    sl_properties_of_widgets.append("isEnabled");
    sl_properties_of_widgets.append("checked");
    sl_properties_of_widgets.append("text");
    sl_properties_of_widgets.append("value");
    sl_properties_of_widgets.append("position");
    sl_properties_of_widgets.append("state");
    sl_properties_of_widgets.append("time");
    sl_properties_of_widgets.append("date");
    sl_properties_of_widgets.append("plainText");
    sl_properties_of_widgets.append("currentIndex");

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
#ifndef LOGGER_ON
    qDebug() << "~MyWidget()" << on;
    //qDebug() << "~MyWidget()" << objectName();
#endif
#endif
}
//--------------------------------------------------------------------------------
bool MyWidget::check_exists_signals(QWidget *parent)
{
    emit trace(Q_FUNC_INFO);

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

    connect(this,   SIGNAL(colorLog(QString, QColor, QColor)),
            parent, SIGNAL(colorLog(QString, QColor, QColor)));

    return true;
}
//--------------------------------------------------------------------------------
// основная функция
void MyWidget::connect_log(QWidget *parent)
{
#ifdef NO_LOG
    // connect(this, SIGNAL(info(QString)),    this, SLOT(log(QString)));
    // connect(this, SIGNAL(debug(QString)),   this, SLOT(log(QString)));
    // connect(this, SIGNAL(error(QString)),   this, SLOT(log(QString)));
    // connect(this, SIGNAL(trace(QString)),   this, SLOT(log(QString)));

    if(parent == nullptr)
    {
#ifdef Q_DEBUG
        qDebug() sl.append("parent is null";
#endif
        return;
    }

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
    Q_ASSERT(src);
    Q_ASSERT(dest);

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
void MyWidget::log(const QString &data)
{
#ifdef QT_DEBUG
    qDebug() << data;
#else
    Q_UNUSED(data)
#endif
}
//--------------------------------------------------------------------------------
// строка или HEX - вот вопрос :)
bool MyWidget::is_letter_or_number(QByteArray ba)
{
    QString str = ba.data();
    int len = str.length();
    for(int n=0; n<len; n++)
    {
        QChar s = str.at(n);
        if(s != '\r' &&
                s != '\n' &&
                s != '\t')
        {
            if(!s.isPrint())    return false;
        }
    }
    return true;
}
//--------------------------------------------------------------------------------
template<typename T>
void MyWidget::set_property_widget(void)
{
    QList<T *> allle = findChildren<T *>();
    foreach (T *obj, allle)
    {
        obj->setProperty(STATE_PROPERTY, true);
        w_lists.append(obj);
    }
}
//--------------------------------------------------------------------------------
void MyWidget::init_w_lists(void)
{
    //emit trace(Q_FUNC_INFO);

    set_property_widget<QLineEdit>();
    set_property_widget<QAbstractSlider>();
    set_property_widget<QAbstractScrollArea>();
    set_property_widget<QAbstractSpinBox>();
    set_property_widget<QComboBox>();
    set_property_widget<QPushButton>();
    set_property_widget<QToolButton>();

    set_property_widget<QLCDNumber>();
}
//--------------------------------------------------------------------------------
void MyWidget::add_widget_to_w_lists(QWidget *widget)
{
    Q_ASSERT(widget);
    w_lists.append(widget);
}
//--------------------------------------------------------------------------------
void MyWidget::lock_interface(void)
{
    //emit trace(Q_FUNC_INFO);
    Q_ASSERT(w_lists.count() > 0);

    foreach (QWidget *btn, w_lists)
    {
        btn->setDisabled(true);
    }
}
//--------------------------------------------------------------------------------
void MyWidget::unlock_interface(void)
{
    //emit trace(Q_FUNC_INFO);
    Q_ASSERT(w_lists.count() > 0);

    foreach (QWidget *widget, w_lists)
    {
        widget->setEnabled(true);
    }
}
//--------------------------------------------------------------------------------
void MyWidget::lock_this_button(void)
{
    emit trace(Q_FUNC_INFO);

    if (QAbstractButton *pb=qobject_cast<QAbstractButton *>(sender()))
    {
        pb->setDisabled(true);
    }
}
//--------------------------------------------------------------------------------
void MyWidget::unlock_this_button(void)
{
    emit trace(Q_FUNC_INFO);

    if (QAbstractButton *pb=qobject_cast<QAbstractButton *>(sender()))
    {
        pb->setEnabled(true);
    }
}
//--------------------------------------------------------------------------------
void MyWidget::block_this_button(bool state)
{
    emit trace(Q_FUNC_INFO);

    if (QAbstractButton *pb=qobject_cast<QAbstractButton *>(sender()))
    {
        pb->setDisabled(state);
    }
}
//--------------------------------------------------------------------------------
template<typename T>
void MyWidget::lock_widget(bool state)
{
    QList<T *> all_obj = findChildren<T *>();
    foreach(T *obj, all_obj)
    {
        if(obj->property(NO_BLOCK).toBool() == false)
        {
            obj->setDisabled(state);
        }
    }
}
//--------------------------------------------------------------------------------
void MyWidget::block_interface(bool state)
{
    //emit trace(Q_FUNC_INFO);

    lock_widget<QAbstractButton>(state);
    lock_widget<QComboBox>(state);
    lock_widget<QCheckBox>(state);
    lock_widget<QLineEdit>(state);
    lock_widget<QDoubleSpinBox>(state);
    lock_widget<QSpinBox>(state);
}
//--------------------------------------------------------------------------------
void MyWidget::block_widget(const QString &name, bool state)
{
    emit trace(Q_FUNC_INFO);

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
    emit trace(Q_FUNC_INFO);

    Q_ASSERT(obj);
    Q_ASSERT(slot_sign);

    if(obj->metaObject()->indexOfSlot(QMetaObject::normalizedSignature(qPrintable(slot_sign))) == -1) return false;
    return true;
}
//--------------------------------------------------------------------------------
bool MyWidget::set_param(QString group_name, QString name, QVariant value)
{
    if(name.isEmpty())  return false;

#ifdef QT_DEBUG
    QString app_name = QString("%1(debug)").arg(APPNAME);
#else
    QString app_name = APPNAME;
#endif

#ifndef SAVE_INI
    QSettings *settings = new QSettings(ORGNAME, app_name);
#else
    QSettings *settings = new QSettings(QString("%1%2").arg(app_name).arg(".ini"), QSettings::IniFormat);
#endif
    Q_ASSERT(settings);

    settings->beginGroup(group_name);
    settings->setValue(name, value);
    settings->endGroup();

    delete settings;
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

#ifdef QT_DEBUG
    QString app_name = QString("%1(debug)").arg(APPNAME);
#else
    QString app_name = APPNAME;
#endif

#ifndef SAVE_INI
    QSettings *settings = new QSettings(ORGNAME, app_name);
#else
    QSettings *settings = new QSettings(QString("%1%2").arg(app_name).arg(".ini"), QSettings::IniFormat);
#endif
    Q_ASSERT(settings);

    settings->beginGroup(group_name);
    QVariant temp = settings->value(name, default_value);
    settings->endGroup();

    delete settings;
    *value = temp;
    return true;
}
//--------------------------------------------------------------------------------
QStringList MyWidget::get_all_param_name(void)
{
#ifdef QT_DEBUG
    QString app_name = QString("%1(debug)").arg(APPNAME);
#else
    QString app_name = APPNAME;
#endif

#ifndef SAVE_INI
    QSettings *settings = new QSettings(ORGNAME, app_name);
#else
    QSettings *settings = new QSettings(QString("%1%2").arg(app_name).arg(".ini"), QSettings::IniFormat);
#endif
    Q_ASSERT(settings);

    settings->beginGroup(PARAMS_GROUP_NAME);
    QStringList temp = settings->allKeys();
    settings->endGroup();

    delete settings;
    return temp;
}
//--------------------------------------------------------------------------------
void MyWidget::block_close(bool state)
{
    emit trace(Q_FUNC_INFO);

    setProperty(FLAG_NO_CLOSE, state);
}
//--------------------------------------------------------------------------------
void MyWidget::closeEvent(QCloseEvent *)
{
    emit trace(Q_FUNC_INFO);

#ifdef QT_DEBUG
    qDebug() << "closeEvent";
#endif
}
//--------------------------------------------------------------------------------
void MyWidget::s_test(void)
{
    emit trace(Q_FUNC_INFO);

    show_objectname();
    // block_wheel();
}
//--------------------------------------------------------------------------------
#ifdef BLOCK_WHEEL
void MyWidget::block_wheel(void)
{
    emit trace(Q_FUNC_INFO);

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
    emit trace(Q_FUNC_INFO);

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
    emit trace(Q_FUNC_INFO);

    QList<QCheckBox *> all_chb = findChildren<QCheckBox *>();

    foreach (check_box cb_obj, data)
    {
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
    emit trace(Q_FUNC_INFO);

    QList<QComboBox *> all_chb = findChildren<QComboBox *>();

    foreach (combo_box cb_obj, data)
    {
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
    emit trace(Q_FUNC_INFO);

    QList<QSpinBox *> all_chb = findChildren<QSpinBox *>();

    foreach (spin_box cb_obj, data)
    {
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
    emit trace(Q_FUNC_INFO);

    QList<QDoubleSpinBox *> all_chb = findChildren<QDoubleSpinBox *>();

    foreach (d_spin_box cb_obj, data)
    {
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
    emit trace(Q_FUNC_INFO);

    Q_ASSERT(w_left);
    Q_ASSERT(w_central);

    QStringList *sl = reinterpret_cast<QStringList *>(w_left);
    if(sl)
    {
        QString text;
        foreach (QString txt, *sl)
        {
            text.append(txt);
        }
        //---
        QWidget *frame = new QWidget();
        QTextEdit *te = new QTextEdit();
        QFont font("Courier", 10);
        te->setFont(font);
        te->setReadOnly(true);
        te->setPlainText(text);
        QVBoxLayout *vbox = new QVBoxLayout();
        vbox->setContentsMargins(0, 0, 0, 0);
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

        QLabel *label = new QLabel();
        label->setPixmap(*main_pixmap);
        label->show();
#endif

        delete frame;
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
    //emit trace(Q_FUNC_INFO);

    Q_ASSERT(tool_bar);
    Q_ASSERT(tool_button);

    tool_button->setIcon(icon);
    tool_button->setText(QObject::tr(text.toLocal8Bit()));
    tool_button->setToolTip(QObject::tr(tool_tip.toLocal8Bit()));
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void MyWidget::check_tooltips(void)
{
    emit trace(Q_FUNC_INFO);

    QList<QAbstractButton *> l_obj = findChildren<QAbstractButton *>();
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
bool MyWidget::fail(const QString &text)
{
    //emit trace(Q_FUNC_INFO);
    emit error(text);
    return false;
}
//--------------------------------------------------------------------------------
bool MyWidget::add_digital_clock(bool add_spacer)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(QApplication::activeWindow());
    if(mw)
    {
        if(add_spacer)
        {
            mw->statusBar()->addWidget(new QWidget(this), 1);
        }

        Digital_clock *digital_clock = new Digital_clock(this);
        mw->statusBar()->addWidget(digital_clock);

        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
bool MyWidget::add_lcd_clock(bool add_spacer)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(QApplication::activeWindow());
    if(mw)
    {
        if(add_spacer)
        {
            mw->statusBar()->addWidget(new QWidget(this), 1);
        }

        LCD_clock *lcd_clock = new LCD_clock(this);
        mw->statusBar()->addWidget(lcd_clock);

        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
void MyWidget::check_file(const QString &orig_file, const QString &copy_file)
{
    if(QFile::exists(copy_file) == false)
    {
        QFileInfo fi(copy_file);
        emit debug(fi.absolutePath());
        QDir dir;
        if(dir.exists(fi.absolutePath()) ==  false)
        {
            dir.mkdir(fi.absolutePath());
        }

        //emit error(QString("%1 не найден").arg(copy_file));
        bool ok = QFile::copy(orig_file, copy_file);
        if(ok)
        {
            emit info(QString("%1 создан").arg(copy_file));
        }
        else
        {
            emit error(QString("%1 НЕ создан").arg(copy_file));
        }
    }
}
//--------------------------------------------------------------------------------
QString MyWidget::get_class_name(const QString &fullname)
{
    QString temp = fullname;

    temp = temp.replace("::", ":")
            .replace("()", "")
            .replace("(void)", "")
            .replace("void ", "");
    QStringList sl = temp.split(":");
    if(sl.count() != 2)
    {
        return fullname;
    }

    QString classname = sl.at(0);
    sl = classname.split(" ");
    return sl.at(sl.count() - 1);
}
//--------------------------------------------------------------------------------
QString MyWidget::get_func_name(const QString &fullname)
{
    QString temp = fullname;

    temp = temp.replace("::", ":")
            .replace("()", "")
            .replace("(void)", "")
            .replace("void ", "");
    QStringList sl = temp.split(":");
    if(sl.count() != 2)
    {
        return fullname;
    }
    return sl.at(1);
}
//--------------------------------------------------------------------------------
void MyWidget::set_all_buttons_no_save(void)
{
    const QWidgetList allWidgets = QApplication::allWidgets();
    for (QWidget *widget : allWidgets)
    {
        QPushButton *btn = (QPushButton *)widget;
        if(btn)
        {
            QString oname = btn->objectName();
            if(oname.length() > 4)
            {
                if(oname.left(4) == "btn_")
                {
                    widget->setProperty(NO_SAVE, true);
                }
            }
        }
    }
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
#ifdef BLOCK_WHEEL
bool MyWidget::eventFilter(QObject*, QEvent* event)
{
    if(event->type() == QEvent::Wheel)
    {
#ifdef Q_DEBUG
        qDebug() sl.append("Wheel event blocked";
#endif
        return true;
    }
    return false;
}
#endif
//--------------------------------------------------------------------------------
void MyWidget::load_widgets(void)
{
    QTimer::singleShot(0, [this]{
        //emit trace(Q_FUNC_INFO);
        QWidgetList widgets = qApp->allWidgets();
        Q_ASSERT(widgets.count() > 0);

        foreach(QWidget *widget, widgets)
        {
            if(is_my_widget(widget->objectName()))
            {
                if(widget->property(NO_SAVE).toBool() == true)
                {
                    continue;
                }

                QString full_name = get_full_objectName(widget);
                Q_ASSERT(full_name.isEmpty() == false);
                beginGroup(full_name);
                load_listwidget_property(widget);
                load_combobox_property(widget);
                load_splitter_property(widget);
                load_mdi_subwindows(widget);

                foreach(QString property, sl_properties_of_widgets)
                {
                    load_property(widget, property);
                }

                endGroup();
            }
        }
    });
}
//--------------------------------------------------------------------------------
void MyWidget::save_widgets(void)
{
    emit trace(Q_FUNC_INFO);

    QWidgetList widgets = qApp->allWidgets();
    Q_ASSERT(widgets.count() > 0);

    foreach(QWidget *widget, widgets)
    {
        Q_ASSERT(widget);
        if(is_my_widget(widget->objectName()))
        {
            if(widget->property(NO_SAVE).toBool() == true)
            {
                continue;
            }

            QString full_name = get_full_objectName(widget);
            Q_ASSERT(full_name.isEmpty() == false);
            beginGroup(full_name);
            save_listwidget_property(widget);
            save_combobox_property(widget);
            save_splitter_property(widget);
            save_mdi_subwindows(widget);

            foreach(QString property, sl_properties_of_widgets)
            {
                save_property(widget, property);
            }

            endGroup();
        }
    }
}
//--------------------------------------------------------------------------------
