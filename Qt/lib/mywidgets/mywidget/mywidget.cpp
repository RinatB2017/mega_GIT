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
#include <QAbstractButton>
#include <QDoubleSpinBox>
#include <QMouseEvent>
#include <QMessageBox>
#include <QToolButton>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QSettings>
#include <QComboBox>
#include <QSplitter>
#include <QSpinBox>
#include <QVariant>
#include <QTimer>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "mywidget.hpp"
#include "logbox.hpp"
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent)
{
    connect_log(parent);
#ifdef QT_DEBUG
    qDebug() << "MyWidget()";
    QTimer::singleShot(100, this, SLOT(debug()));
#endif
}
//--------------------------------------------------------------------------------
MyWidget::~MyWidget()
{
#ifdef QT_DEBUG
    qDebug() << "~MyWidget()";
#endif
}
//--------------------------------------------------------------------------------
// основная функция
void MyWidget::connect_log(QWidget *parent)
{
#ifdef NO_LOG
    connect(this, SIGNAL(info(QString)),    this, SLOT(log(QString)));
    connect(this, SIGNAL(debug(QString)),   this, SLOT(log(QString)));
    connect(this, SIGNAL(error(QString)),   this, SLOT(log(QString)));
    connect(this, SIGNAL(trace(QString)),   this, SLOT(log(QString)));

    Q_UNUSED(parent);
#else

    if(parent)
    {
        connect(this, SIGNAL(info(QString)),    parent, SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)),   parent, SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)),   parent, SIGNAL(error(QString)));
        connect(this, SIGNAL(trace(QString)),   parent, SIGNAL(trace(QString)));

        connect(this,   SIGNAL(syslog(int,QString,QString)),
                parent, SIGNAL(syslog(int,QString,QString)));
    }
    else
    {
        connect(this, SIGNAL(info(QString)),    this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)),   this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)),   this, SLOT(log(QString)));
        connect(this, SIGNAL(trace(QString)),   this, SLOT(log(QString)));
    }
#endif
}
//--------------------------------------------------------------------------------
void MyWidget::log(const QString data)
{
    qDebug() << data;
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
    //emit debug(QString("w_lists.count %1").arg(w_lists.count()));
}
//--------------------------------------------------------------------------------
void MyWidget::lock_interface(void)
{
#if 1
    foreach (QWidget *btn, w_lists)
    {
        //emit debug(QString("%1").arg(btn->isEnabled() ? "true" : "false"));
        btn->setProperty("state", btn->isEnabled());
        btn->setDisabled(true);
    }

#else
    for(int n=0; n<w_lists.count(); n++)
    {
        //emit debug(QString("%1").arg(w_lists[n]->isEnabled() ? "true" : "false"));
        w_lists[n]->setProperty("state", w_lists[n]->isEnabled());
        w_lists[n]->setDisabled(true);
    }
#endif
}
//--------------------------------------------------------------------------------
void MyWidget::unlock_interface(void)
{
#if 1
    foreach (QWidget *btn, w_lists)
    {
        bool ok = btn->property("state").toBool();
        //emit debug(QString("%1").arg(ok ? "true" : "false"));
        btn->setEnabled(ok);
    }
#else
    for(int n=0; n<w_lists.count(); n++)
    {
        bool ok = w_lists[n]->property("state").toBool();
        emit debug(QString("%1").arg(ok ? "true" : "false"));
        w_lists[n]->setEnabled(ok);
    }
#endif
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
        if(obj->property("no_block").toBool() == false)
        {
            obj->setDisabled(state);
        }
    }
    //---
    QList<QComboBox *> all_cb = findChildren<QComboBox *>();
    foreach(QComboBox *obj, all_cb)
    {
        if(obj->property("no_block").toBool() == false)
        {
            obj->setDisabled(state);
        }
    }
    //---
    QList<QDoubleSpinBox *> all_dsb = findChildren<QDoubleSpinBox *>();
    foreach(QDoubleSpinBox *obj, all_dsb)
    {
        if(obj->property("no_block").toBool() == false)
        {
            obj->setDisabled(state);
        }
    }
    //---
    QList<QSpinBox *> all_sb = findChildren<QSpinBox *>();
    foreach(QSpinBox *obj, all_sb)
    {
        if(obj->property("no_block").toBool() == false)
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
            if(obj->property("no_block").toBool() == false)
            {
                obj->setDisabled(state);
            }
        }
    }
}
//--------------------------------------------------------------------------------
bool MyWidget::is_slot_exists(QWidget *obj, const char *slot_sign)
{
    if(obj == NULL) return false;
    if(obj->metaObject()->indexOfSlot(QMetaObject::normalizedSignature(qPrintable(slot_sign))) == -1) return false;
    return true;
}
//--------------------------------------------------------------------------------
void MyWidget::load_config(void)
{
    qDebug() << "load_config";
    load_widgets("MyWidget");
}
//--------------------------------------------------------------------------------
void MyWidget::save_config(void)
{
    qDebug() << "save_config";
    save_widgets("MyWidget");
}
//--------------------------------------------------------------------------------
bool MyWidget::set_param(QString group_name, QString name, QVariant value)
{
    if(name.isEmpty())  return false;

    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return false;
    }
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
    if(name.isEmpty())  return false;

    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return false;
    }
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
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return QStringList();
    }
    settings->beginGroup(PARAMS_GROUP_NAME);
    QStringList temp = settings->allKeys();
    settings->endGroup();

    settings->deleteLater();
    return temp;
}
//--------------------------------------------------------------------------------
void MyWidget::load_QCheckBox(QString group_name)
{
    QList<QCheckBox *> widgets = findChildren<QCheckBox *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach (QCheckBox *obj, widgets)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->setChecked(settings->value("checked", false).toBool());
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QCheckBox(QString group_name)
{
    QList<QCheckBox *> allobj = findChildren<QCheckBox *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach(QCheckBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("checked", QVariant(obj->isChecked()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QComboBox(QString group_name)
{
    QList<QComboBox *> allobj = findChildren<QComboBox *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach (QComboBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->setCurrentIndex(settings->value("currentindex", 0).toInt());
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QComboBox(QString group_name)
{
    QList<QComboBox *> allobj = findChildren<QComboBox *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach(QComboBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("currentindex", QVariant(obj->currentIndex()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QPushButton(QString group_name)
{
    QList<QPushButton *> allobj = findChildren<QPushButton *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach (QPushButton *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->setText(settings->value("text", "").toString());
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QPushButton(QString group_name)
{
    QList<QPushButton *> allobj = findChildren<QPushButton *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach(QPushButton *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("text", QVariant(obj->text()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QToolButton(QString group_name)
{
    QList<QToolButton *> allobj = findChildren<QToolButton *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach (QToolButton *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->setText(settings->value("text", "").toString());
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QToolButton(QString group_name)
{
    QList<QToolButton *> allobj = findChildren<QToolButton *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach(QToolButton *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("text", QVariant(obj->text()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QTextEdit(QString group_name)
{
    QList<QTextEdit *> allobj = findChildren<QTextEdit *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach (QTextEdit *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->setText(settings->value("text", "").toString());
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QTextEdit(QString group_name)
{
    QList<QTextEdit *> allobj = findChildren<QTextEdit *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach(QTextEdit *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("text", QVariant(obj->toPlainText()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QLineEdit(QString group_name)
{
    QList<QLineEdit *> allobj = findChildren<QLineEdit *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach (QLineEdit *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->setText(settings->value("text", "").toString());
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QLineEdit(QString group_name)
{
    QList<QLineEdit *> allobj = findChildren<QLineEdit *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach(QLineEdit *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("text", QVariant(obj->text()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QSpinBox(QString group_name)
{
    QList<QSpinBox *> allobj = findChildren<QSpinBox *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach (QSpinBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            if(obj->property("no_save") != 0)
            {
                settings->beginGroup(obj->objectName());
                obj->setValue(settings->value("value", 0).toInt());
                settings->endGroup();
            }
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QSpinBox(QString group_name)
{
    QList<QSpinBox *> allobj = findChildren<QSpinBox *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach(QSpinBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("value", QVariant(obj->value()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QDoubleSpinBox(QString group_name)
{
    QList<QDoubleSpinBox *> allobj = findChildren<QDoubleSpinBox *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach (QDoubleSpinBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->setValue(settings->value("value", 0).toFloat());
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QDoubleSpinBox(QString group_name)
{
    QList<QDoubleSpinBox *> allobj = findChildren<QDoubleSpinBox *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach(QDoubleSpinBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("value", QVariant(obj->value()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QSlider(QString group_name)
{
    QList<QSlider *> allobj = findChildren<QSlider *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach (QSlider *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->setSliderPosition(settings->value("position", 0).toDouble());
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QSlider(QString group_name)
{
    QList<QSlider *> allobj = findChildren<QSlider *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach(QSlider *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("position", QVariant(obj->value()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QSplitter(QString group_name)
{
    QList<QSplitter *> allobj = findChildren<QSplitter *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach (QSplitter *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->restoreState(settings->value("state", 0).toByteArray());
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QSplitter(QString group_name)
{
    QList<QSplitter *> allobj = findChildren<QSplitter *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach(QSplitter *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("state", QVariant(obj->saveState()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_widgets(QString group_name)
{
#ifdef SAVE_WIDGETS_CHECKBOX
    load_QCheckBox(group_name);
#endif
#ifdef SAVE_WIDGETS_COMBOBOX
    load_QComboBox(group_name);
#endif
#ifdef SAVE_WIDGETS_PUSHBUTTON
    load_QPushButton(group_name);
#endif
#ifdef SAVE_WIDGETS_TOOLBUTTON
    load_QToolButton(group_name);
#endif
#ifdef SAVE_WIDGETS_SPINBOX
    load_QSpinBox(group_name);
#endif
#ifdef SAVE_WIDGETS_DOUBLESPINBOX
    load_QDoubleSpinBox(group_name);
#endif
#ifdef SAVE_WIDGETS_SLIDER
    load_QSlider(group_name);
#endif
#ifdef SAVE_WIDGETS_SPLITTER
    load_QSplitter(group_name);
#endif
#ifdef SAVE_WIDGETS_TEXTEDIT
    load_QTextEdit(group_name);
#endif
#ifdef SAVE_WIDGETS_LINEEDIT
    load_QLineEdit(group_name);
#endif
    Q_UNUSED(group_name);
}
//--------------------------------------------------------------------------------
void MyWidget::save_widgets(QString group_name)
{
#ifdef SAVE_WIDGETS_CHECKBOX
    save_QCheckBox(group_name);
#endif
#ifdef SAVE_WIDGETS_COMBOBOX
    save_QComboBox(group_name);
#endif
#ifdef SAVE_WIDGETS_PUSHBUTTON
    save_QPushButton(group_name);
#endif
#ifdef SAVE_WIDGETS_TOOLBUTTON
    save_QToolButton(group_name);
#endif
#ifdef SAVE_WIDGETS_SPINBOX
    save_QSpinBox(group_name);
#endif
#ifdef SAVE_WIDGETS_DOUBLESPINBOX
    save_QDoubleSpinBox(group_name);
#endif
#ifdef SAVE_WIDGETS_SLIDER
    save_QSlider(group_name);
#endif
#ifdef SAVE_WIDGETS_SPLITTER
    save_QSplitter(group_name);
#endif
#ifdef SAVE_WIDGETS_TEXTEDIT
    save_QTextEdit(group_name);
#endif
#ifdef SAVE_WIDGETS_LINEEDIT
    save_QLineEdit(group_name);
#endif
    Q_UNUSED(group_name);
}
//--------------------------------------------------------------------------------
void MyWidget::block_close(bool state)
{
    setProperty("flag_no_close", state);
}
//--------------------------------------------------------------------------------
void MyWidget::closeEvent(QCloseEvent *)
{
    qDebug() << "closeEvent";
}
//--------------------------------------------------------------------------------
void MyWidget::debug(void)
{
    show_objectname();
    block_wheel();
}
//--------------------------------------------------------------------------------
void MyWidget::block_wheel(void)
{
    QList<QWidget *> all_obj = findChildren<QWidget *>();
    foreach(QWidget *obj, all_obj)
    {
        if(!obj->objectName().isEmpty())
        {
            obj->installEventFilter(this);
        }
    }
}
//--------------------------------------------------------------------------------
void MyWidget::show_objectname(void)
{
    QList<QAbstractSpinBox *> all_sb = findChildren<QAbstractSpinBox *>();
    foreach(QAbstractSpinBox *obj, all_sb)
    {
        if(obj->objectName().isEmpty() == false)
        {
            obj->setToolTip(obj->objectName());
        }
    }
    QList<QTextEdit *> all_te = findChildren<QTextEdit *>();
    foreach(QTextEdit *obj, all_te)
    {
        if(obj->objectName().isEmpty() == false)
        {
            obj->setToolTip(obj->objectName());
        }
    }
    QList<QLineEdit *> all_le = findChildren<QLineEdit *>();
    foreach(QLineEdit *obj, all_le)
    {
        if(obj->objectName().isEmpty() == false)
        {
            obj->setToolTip(obj->objectName());
        }
    }
    QList<QComboBox *> all_cb = findChildren<QComboBox *>();
    foreach(QComboBox *obj, all_cb)
    {
        if(obj->objectName().isEmpty() == false)
        {
            obj->setToolTip(obj->objectName());
        }
    }
    QList<QCheckBox *> all_chb = findChildren<QCheckBox *>();
    foreach(QCheckBox *obj, all_chb)
    {
        if(obj->objectName().isEmpty() == false)
        {
            obj->setToolTip(obj->objectName());
        }
    }
    QList<QAbstractButton *> all_btn = findChildren<QAbstractButton *>();
    foreach(QAbstractButton *obj, all_btn)
    {
        if(obj->objectName().isEmpty() == false)
        {
            obj->setToolTip(obj->objectName());
        }
    }
    QList<QAbstractSlider *> all_sl = findChildren<QAbstractSlider *>();
    foreach(QAbstractSlider *obj, all_sl)
    {
        if(obj->objectName().isEmpty() == false)
        {
            obj->setToolTip(obj->objectName());
        }
    }
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
int MyWidget::messagebox_noicon(const QString title,
                                const QString text,
                                unsigned int width)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::NoIcon);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Ok);
    QSpacerItem* horizontalSpacer = new QSpacerItem(width, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = (QGridLayout*)msgBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    return msgBox.exec();
}
//--------------------------------------------------------------------------------
int MyWidget::messagebox_info(const QString title,
                              const QString text,
                              unsigned int width)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Ok);
    QSpacerItem* horizontalSpacer = new QSpacerItem(width, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = (QGridLayout*)msgBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    return msgBox.exec();
}
//--------------------------------------------------------------------------------
int MyWidget::messagebox_question(const QString title,
                                  const QString text,
                                  unsigned int width)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    QSpacerItem* horizontalSpacer = new QSpacerItem(width, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = (QGridLayout*)msgBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    return msgBox.exec();
}
//--------------------------------------------------------------------------------
int MyWidget::messagebox_critical(const QString title,
                                  const QString text,
                                  unsigned int width)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Ok);
    QSpacerItem* horizontalSpacer = new QSpacerItem(width, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = (QGridLayout*)msgBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    return msgBox.exec();
}
//--------------------------------------------------------------------------------
int MyWidget::messagebox_warning(const QString title,
                                 const QString text,
                                 unsigned int width)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Ok);
    QSpacerItem* horizontalSpacer = new QSpacerItem(width, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = (QGridLayout*)msgBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    return msgBox.exec();
}
//--------------------------------------------------------------------------------
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
void MyWidget::save_checkBox(QString group_name, QList<QCheckBox *> data)
{
    QList<QCheckBox *> all_chb = findChildren<QCheckBox *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);

    settings->beginGroup(group_name);

    foreach (QCheckBox *cb_obj, data) {
        foreach(QCheckBox *obj, all_chb)
        {
            if(obj->objectName() == cb_obj->objectName())
            {
                settings->beginGroup(obj->objectName());
                settings->setValue("checked", QVariant(obj->isChecked()));
                settings->endGroup();
            }
        }
    }
}
//--------------------------------------------------------------------------------
void MyWidget::save_comboBox(QString group_name, QList<QComboBox *> data)
{
    QList<QComboBox *> all_chb = findChildren<QComboBox *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);

    settings->beginGroup(group_name);

    foreach (QComboBox *cb_obj, data) {
        foreach(QComboBox *obj, all_chb)
        {
            if(obj->objectName() == cb_obj->objectName())
            {
                settings->beginGroup(obj->objectName());
                settings->setValue("index", QVariant(obj->currentIndex()));
                settings->endGroup();
            }
        }
    }
}
//--------------------------------------------------------------------------------
void MyWidget::save_spinBox(QString group_name, QList<QSpinBox *> data)
{
    QList<QSpinBox *> all_chb = findChildren<QSpinBox *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);

    settings->beginGroup(group_name);

    foreach (QSpinBox *sb_obj, data) {
        foreach(QSpinBox *obj, all_chb)
        {
            if(obj->objectName() == sb_obj->objectName())
            {
                settings->beginGroup(obj->objectName());
                settings->setValue("value", QVariant(obj->value()));
                settings->endGroup();
            }
        }
    }
}
//--------------------------------------------------------------------------------
void MyWidget::save_d_spinBox(QString group_name, QList<QDoubleSpinBox *> data)
{
    QList<QDoubleSpinBox *> all_chb = findChildren<QDoubleSpinBox *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);

    settings->beginGroup(group_name);

    foreach (QDoubleSpinBox *sb_obj, data) {
        foreach(QDoubleSpinBox *obj, all_chb)
        {
            if(obj->objectName() == sb_obj->objectName())
            {
                settings->beginGroup(obj->objectName());
                settings->setValue("value", QVariant(obj->value()));
                settings->endGroup();
            }
        }
    }
    settings->endGroup();
    settings->deleteLater();
}
//--------------------------------------------------------------------------------
#include <QStringList>
#include <QPainter>
bool MyWidget::create_pixmap(QWidget *w_left, QWidget *w_central)
{
    if(w_left == nullptr)
    {
        emit error("w_left is null");
        return false;
    }
    if(w_central == nullptr)
    {
        emit error("w_central is null");
        return false;
    }

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
        QVBoxLayout *vbox = new QVBoxLayout;
        vbox->setMargin(0);
        vbox->addWidget(te);
        frame->setLayout(vbox);
        frame->setFixedSize(te->width(), w_central->height());
        //---
        QPixmap p_central = w_central->grab(QRect(0, 0, w_central->width(), w_central->height()));
        QPixmap left_pixmap = ((QWidget *)frame)->grab(QRect(0, 0, frame->width(), frame->height()));
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
    tool_button->setText(text);
    tool_button->setToolTip(tool_tip);
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
bool MyWidget::eventFilter(QObject*, QEvent* event)
{
    QMouseEvent *mouseEvent = (QMouseEvent *) event;
    //---
    if(mouseEvent->type() == QMouseEvent::Wheel)
    {
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
bool MyWidget::close(void)
{
    return is_close;
}
//--------------------------------------------------------------------------------
