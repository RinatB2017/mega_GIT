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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
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
    QWidget(parent)
{
#ifndef SAVE_INI
    settings = new QSettings(ORGNAME, APPNAME);
#else
    settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
#endif

#ifndef RS232_LOG
//    if(parent)
        connect_log(parent);
//    else
//        connect_log(topLevelWidget());
#endif
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
    if(settings)
    {
        settings->deleteLater();
    }
}
//--------------------------------------------------------------------------------
bool MyWidget::check_exists_signals(QWidget *parent)
{
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

    connect(this, SIGNAL(info(QString)),    parent, SIGNAL(info(QString)));
    connect(this, SIGNAL(debug(QString)),   parent, SIGNAL(debug(QString)));
    connect(this, SIGNAL(error(QString)),   parent, SIGNAL(error(QString)));
    connect(this, SIGNAL(trace(QString)),   parent, SIGNAL(trace(QString)));

    connect(this,   SIGNAL(colorLog(QString, QColor, QColor)),  parent, SIGNAL(colorLog(QString, QColor, QColor)));

    return true;
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

    connect(this, SIGNAL(info(QString)),    parent, SIGNAL(info(QString)));
    connect(this, SIGNAL(debug(QString)),   parent, SIGNAL(debug(QString)));
    connect(this, SIGNAL(error(QString)),   parent, SIGNAL(error(QString)));
    connect(this, SIGNAL(trace(QString)),   parent, SIGNAL(trace(QString)));

    Q_UNUSED(parent);
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

#if 0
    if(parent)
    {
        int m_info  = parent->metaObject()->indexOfSignal("info(QString)");
        int m_debug = parent->metaObject()->indexOfSignal("debug(QString)");
        int m_error = parent->metaObject()->indexOfSignal("error(QString)");
        int m_trace = parent->metaObject()->indexOfSignal("trace(QString)");
        int m_colorlog = parent->metaObject()->indexOfSignal("colorLog(QString,QColor,QColor)");

        if(m_info != -1)  connect(this, SIGNAL(info(QString)),    parent, SIGNAL(info(QString)));
        else connect(this,  SIGNAL(info(QString)),      this, SLOT(log(QString)));
        if(m_debug != -1) connect(this, SIGNAL(debug(QString)),   parent, SIGNAL(debug(QString)));
        else connect(this,  SIGNAL(debug(QString)),     this, SLOT(log(QString)));
        if(m_error != -1) connect(this, SIGNAL(error(QString)),   parent, SIGNAL(error(QString)));
        else connect(this,  SIGNAL(error(QString)),     this, SLOT(log(QString)));
        if(m_trace != -1) connect(this, SIGNAL(trace(QString)),   parent, SIGNAL(trace(QString)));
        else connect(this,  SIGNAL(trace(QString)),     this, SLOT(log(QString)));

        if(m_colorlog != -1) connect(this,   SIGNAL(colorLog(QString,QColor,QColor)),   parent, SIGNAL(colorLog(QString,QColor,QColor)));
    }
    else
    {
        connect(this, SIGNAL(info(QString)),    this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)),   this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)),   this, SLOT(log(QString)));
        connect(this, SIGNAL(trace(QString)),   this, SLOT(log(QString)));

        connect(this,   SIGNAL(colorLog(QColor,QString)),   this, SLOT(log(QString)));
    }
#endif
#endif
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
void MyWidget::lock_interface(void)
{
#if 1
    foreach (QWidget *btn, w_lists)
    {
        btn->setProperty("state", btn->isEnabled());
        btn->setDisabled(true);
    }
#else
    for(int n=0; n<w_lists.count(); n++)
    {
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
        btn->setEnabled(ok);
    }
#else
    for(int n=0; n<w_lists.count(); n++)
    {
        bool ok = w_lists[n]->property("state").toBool();
        emit debug(QString("property(state) is %1").arg(ok ? "true" : "false"));
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

#ifndef SAVE_INI
    QSettings *settings = new QSettings(ORGNAME, APPNAME);
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

#ifndef SAVE_INI
    QSettings *settings = new QSettings(ORGNAME, APPNAME);
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
#ifndef SAVE_INI
    QSettings *settings = new QSettings(ORGNAME, APPNAME);
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
void MyWidget::load_QRadioButton(QString group_name)
{
#ifdef USE_TOPLEVELWIDGETS
    QList<QRadioButton *> widgets = topLevelWidget()->findChildren<QRadioButton *>();
#else
    QList<QRadioButton *> widgets = findChildren<QRadioButton *>();
#endif
    Q_CHECK_PTR(settings);

    settings->beginGroup(group_name);
    foreach (QRadioButton *obj, widgets)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->setChecked(settings->value("checked", false).toBool());
            settings->endGroup();
        }
    }
    settings->endGroup();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QRadioButton(QString group_name)
{
#ifdef USE_TOPLEVELWIDGETS
    QList<QRadioButton *> allobj = topLevelWidget()->findChildren<QRadioButton *>();
#else
    QList<QRadioButton *> allobj = findChildren<QRadioButton *>();
#endif
    Q_CHECK_PTR(settings);

    settings->beginGroup(group_name);
    foreach(QRadioButton *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("checked", QVariant(obj->isChecked()));
            settings->endGroup();
        }
    }
    settings->endGroup();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QCheckBox(QString group_name)
{
#ifdef USE_TOPLEVELWIDGETS
    QList<QCheckBox *> widgets = topLevelWidget()->findChildren<QCheckBox *>();
#else
    QList<QCheckBox *> widgets = findChildren<QCheckBox *>();
#endif
    Q_CHECK_PTR(settings);

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
}
//--------------------------------------------------------------------------------
void MyWidget::save_QCheckBox(QString group_name)
{
#ifdef USE_TOPLEVELWIDGETS
    QList<QCheckBox *> allobj = topLevelWidget()->findChildren<QCheckBox *>();
#else
    QList<QCheckBox *> allobj = findChildren<QCheckBox *>();
#endif
    Q_CHECK_PTR(settings);

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
}
//--------------------------------------------------------------------------------
void MyWidget::load_QComboBox(QString group_name)
{
#ifdef USE_TOPLEVELWIDGETS
    QList<QComboBox *> allobj = topLevelWidget()->findChildren<QComboBox *>();
#else
    QList<QComboBox *> allobj = findChildren<QComboBox *>();
#endif
    Q_CHECK_PTR(settings);

    settings->beginGroup(group_name);
    foreach (QComboBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            int size = settings->beginReadArray(obj->objectName());
            for(int n=0; n<size; n++)
            {
                settings->setArrayIndex(n);
                obj->addItem(settings->value("value").toString());
            }
            settings->endArray();
            obj->setCurrentIndex(settings->value("currentindex", 0).toInt());
        }
    }

    settings->endGroup();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QComboBox(QString group_name)
{
#ifdef USE_TOPLEVELWIDGETS
    QList<QComboBox *> allobj = topLevelWidget()->findChildren<QComboBox *>();
#else
    QList<QComboBox *> allobj = findChildren<QComboBox *>();
#endif
    Q_CHECK_PTR(settings);

    settings->beginGroup(group_name);
    foreach(QComboBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->setValue("currentindex", QVariant(obj->currentIndex()));
            settings->beginWriteArray(obj->objectName(), obj->count());
            for(int n=0; n<obj->count(); n++)
            {
                settings->setArrayIndex(n);
                obj->setCurrentIndex(n);
                settings->setValue("value", obj->currentText());
            }
            settings->endArray();
        }
    }

    settings->endGroup();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QPushButton(QString group_name)
{
#ifdef USE_TOPLEVELWIDGETS
    QList<QPushButton *> allobj = topLevelWidget()->findChildren<QPushButton *>();
#else
    QList<QPushButton *> allobj = findChildren<QPushButton *>();
#endif
    Q_CHECK_PTR(settings);

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
}
//--------------------------------------------------------------------------------
void MyWidget::save_QPushButton(QString group_name)
{
#ifdef USE_TOPLEVELWIDGETS
    QList<QPushButton *> allobj = topLevelWidget()->findChildren<QPushButton *>();
#else
    QList<QPushButton *> allobj = findChildren<QPushButton *>();
#endif
    Q_CHECK_PTR(settings);

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
}
//--------------------------------------------------------------------------------
void MyWidget::load_QToolButton(QString group_name)
{
#ifdef USE_TOPLEVELWIDGETS
    QList<QToolButton *> allobj = topLevelWidget()->findChildren<QToolButton *>();
#else
    QList<QToolButton *> allobj = findChildren<QToolButton *>();
#endif
    Q_CHECK_PTR(settings);

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
}
//--------------------------------------------------------------------------------
void MyWidget::save_QToolButton(QString group_name)
{
#ifdef USE_TOPLEVELWIDGETS
    QList<QToolButton *> allobj = topLevelWidget()->findChildren<QToolButton *>();
#else
    QList<QToolButton *> allobj = findChildren<QToolButton *>();
#endif
    Q_CHECK_PTR(settings);

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
}
//--------------------------------------------------------------------------------
void MyWidget::load_QTextEdit(QString group_name)
{
#ifdef USE_TOPLEVELWIDGETS
    QList<QTextEdit *> allobj = topLevelWidget()->findChildren<QTextEdit *>();
#else
    QList<QTextEdit *> allobj = findChildren<QTextEdit *>();
#endif
    Q_CHECK_PTR(settings);

    settings->beginGroup(group_name);
    foreach (QTextEdit *obj, allobj)
    {
        QString o_name = obj->objectName();
        if(!o_name.isEmpty())
        {
            if(o_name.left(3) == "te_") //TODO костыль
            {
                settings->beginGroup(o_name);
                //qDebug() << "objectName ===>" << obj->objectName();
                //qDebug() << "text       ===>" << settings->value("text", "").toString();
                obj->setPlainText(settings->value("text", "").toString());
                settings->endGroup();
            }
        }
    }
    settings->endGroup();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QTextEdit(QString group_name)
{
#ifdef USE_TOPLEVELWIDGETS
    QList<QTextEdit *> allobj = topLevelWidget()->findChildren<QTextEdit *>();
#else
    QList<QTextEdit *> allobj = findChildren<QTextEdit *>();
#endif
    Q_CHECK_PTR(settings);

    settings->beginGroup(group_name);
    foreach(QTextEdit *obj, allobj)
    {
        QString o_name = obj->objectName();
        if(!o_name.isEmpty())
        {
            if(o_name.left(3) == "te_") //TODO костыль
            {
                settings->beginGroup(o_name);
                settings->setValue("text", QVariant(obj->toPlainText()));
                settings->endGroup();
            }
        }
    }
    settings->endGroup();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QPlainTextEdit(QString group_name)
{
#ifdef USE_TOPLEVELWIDGETS
    QList<QPlainTextEdit *> allobj = topLevelWidget()->findChildren<QPlainTextEdit *>();
#else
    QList<QPlainTextEdit *> allobj = findChildren<QPlainTextEdit *>();
#endif
    Q_CHECK_PTR(settings);

    settings->beginGroup(group_name);
    foreach (QPlainTextEdit *obj, allobj)
    {
        QString o_name = obj->objectName();
        if(!o_name.isEmpty())
        {
            if(o_name.left(3) == "te_") //TODO костыль
            {
                settings->beginGroup(o_name);
                //qDebug() << "objectName ===>" << obj->objectName();
                //qDebug() << "text       ===>" << settings->value("text", "").toString();
                obj->setPlainText(settings->value("text", "").toString());
                settings->endGroup();
            }
        }
    }
    settings->endGroup();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QPlainTextEdit(QString group_name)
{
#ifdef USE_TOPLEVELWIDGETS
    QList<QPlainTextEdit *> allobj = topLevelWidget()->findChildren<QPlainTextEdit *>();
#else
    QList<QPlainTextEdit *> allobj = findChildren<QPlainTextEdit *>();
#endif
    Q_CHECK_PTR(settings);

    settings->beginGroup(group_name);
    foreach(QPlainTextEdit *obj, allobj)
    {
        QString o_name = obj->objectName();
        if(!o_name.isEmpty())
        {
            if(o_name.left(3) == "te_") //TODO костыль
            {
                settings->beginGroup(o_name);
                settings->setValue("text", QVariant(obj->toPlainText()));
                settings->endGroup();
            }
        }
    }
    settings->endGroup();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QLineEdit(QString group_name)
{
#ifdef USE_TOPLEVELWIDGETS
    QList<QLineEdit *> allobj = topLevelWidget()->findChildren<QLineEdit *>();
#else
    QList<QLineEdit *> allobj = findChildren<QLineEdit *>();
#endif
    Q_CHECK_PTR(settings);

    settings->beginGroup(group_name);
    foreach (QLineEdit *obj, allobj)
    {
        QString o_name = obj->objectName();
        if(!o_name.isEmpty())
        {
            if(o_name.left(3) == "le_") //TODO костыль
            {
                settings->beginGroup(obj->objectName());
                obj->setText(settings->value("text", "").toString());
                settings->endGroup();
            }
        }
    }
    settings->endGroup();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QLineEdit(QString group_name)
{
#ifdef USE_TOPLEVELWIDGETS
    QList<QLineEdit *> allobj = topLevelWidget()->findChildren<QLineEdit *>();
#else
    QList<QLineEdit *> allobj = findChildren<QLineEdit *>();
#endif
    Q_CHECK_PTR(settings);

    settings->beginGroup(group_name);
    foreach(QLineEdit *obj, allobj)
    {
        QString o_name = obj->objectName();
        if(!o_name.isEmpty())
        {
            if(o_name.left(3) == "le_") //TODO костыль
            {
                settings->beginGroup(obj->objectName());
                settings->setValue("text", QVariant(obj->text()));
                settings->endGroup();
            }
        }
    }
    settings->endGroup();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QSpinBox(QString group_name)
{
#ifdef USE_TOPLEVELWIDGETS
    QList<QSpinBox *> allobj = topLevelWidget()->findChildren<QSpinBox *>();
#else
    QList<QSpinBox *> allobj = findChildren<QSpinBox *>();
#endif
    Q_CHECK_PTR(settings);

    settings->beginGroup(group_name);
    foreach (QSpinBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->setValue(settings->value("value", 0).toInt());
            settings->endGroup();
        }
    }
    settings->endGroup();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QSpinBox(QString group_name)
{
#ifdef USE_TOPLEVELWIDGETS
    QList<QSpinBox *> allobj = topLevelWidget()->findChildren<QSpinBox *>();
#else
    QList<QSpinBox *> allobj = findChildren<QSpinBox *>();
#endif
    Q_CHECK_PTR(settings);

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
}
//--------------------------------------------------------------------------------
void MyWidget::load_QDoubleSpinBox(QString group_name)
{
#ifdef USE_TOPLEVELWIDGETS
    QList<QDoubleSpinBox *> allobj = topLevelWidget()->findChildren<QDoubleSpinBox *>();
#else
    QList<QDoubleSpinBox *> allobj = findChildren<QDoubleSpinBox *>();
#endif
    Q_CHECK_PTR(settings);

    settings->beginGroup(group_name);
    foreach (QDoubleSpinBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->setValue(settings->value("value", 0).toDouble());
            settings->endGroup();
        }
    }
    settings->endGroup();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QDoubleSpinBox(QString group_name)
{
#ifdef USE_TOPLEVELWIDGETS
    QList<QDoubleSpinBox *> allobj = topLevelWidget()->findChildren<QDoubleSpinBox *>();
#else
    QList<QDoubleSpinBox *> allobj = findChildren<QDoubleSpinBox *>();
#endif
    Q_CHECK_PTR(settings);

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
}
//--------------------------------------------------------------------------------
void MyWidget::load_QSlider(QString group_name)
{
#ifdef USE_TOPLEVELWIDGETS
    QList<QSlider *> allobj = topLevelWidget()->findChildren<QSlider *>();
#else
    QList<QSlider *> allobj = findChildren<QSlider *>();
#endif
    Q_CHECK_PTR(settings);

    settings->beginGroup(group_name);
    foreach (QSlider *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->setSliderPosition(settings->value("position", 0).toInt());
            settings->endGroup();
        }
    }
    settings->endGroup();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QSlider(QString group_name)
{
#ifdef USE_TOPLEVELWIDGETS
    QList<QSlider *> allobj = topLevelWidget()->findChildren<QSlider *>();
#else
    QList<QSlider *> allobj = findChildren<QSlider *>();
#endif
    Q_CHECK_PTR(settings);

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
}
//--------------------------------------------------------------------------------
void MyWidget::load_QSplitter(QString group_name)
{
#ifdef USE_TOPLEVELWIDGETS
    QList<QSplitter *> allobj = topLevelWidget()->findChildren<QSplitter *>();
#else
    QList<QSplitter *> allobj = findChildren<QSplitter *>();
#endif
    Q_CHECK_PTR(settings);

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
}
//--------------------------------------------------------------------------------
void MyWidget::save_QSplitter(QString group_name)
{
#ifdef USE_TOPLEVELWIDGETS
    QList<QSplitter *> allobj = topLevelWidget()->findChildren<QSplitter *>();
#else
    QList<QSplitter *> allobj = findChildren<QSplitter *>();
#endif
    Q_CHECK_PTR(settings);

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
}
//--------------------------------------------------------------------------------
void MyWidget::load_QTimeEdit(QString group_name)
{
#ifdef USE_TOPLEVELWIDGETS
    QList<QTimeEdit *> allobj = topLevelWidget()->findChildren<QTimeEdit *>();
#else
    QList<QTimeEdit *> allobj = findChildren<QTimeEdit *>();
#endif
    Q_CHECK_PTR(settings);

    settings->beginGroup(group_name);
    foreach (QTimeEdit *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->setDateTime(settings->value("datetime", 0).toDateTime());
            settings->endGroup();
        }
    }
    settings->endGroup();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QTimeEdit(QString group_name)
{
#ifdef USE_TOPLEVELWIDGETS
    QList<QTimeEdit *> allobj = topLevelWidget()->findChildren<QTimeEdit *>();
#else
    QList<QTimeEdit *> allobj = findChildren<QTimeEdit *>();
#endif
    Q_CHECK_PTR(settings);

    settings->beginGroup(group_name);
    foreach(QTimeEdit *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("datetime", QVariant(obj->dateTime()));
            settings->endGroup();
        }
    }
    settings->endGroup();
}
//--------------------------------------------------------------------------------
void MyWidget::load_widgets(QString group_name)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw)
    {
        mw->load_setting();
    }

#ifdef SAVE_WIDGETS_RADIOBUTTON
    load_QRadioButton(group_name);
#endif
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
#ifdef SAVE_WIDGETS_TIMEEDIT
    load_QTimeEdit(group_name);
#endif
#ifdef SAVE_WIDGETS_PLAINTEXTEDIT
    load_QPlainTextEdit(group_name);
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
#ifdef SAVE_WIDGETS_RADIOBUTTON
    save_QRadioButton(group_name);
#endif
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
#ifdef SAVE_WIDGETS_TIMEEDIT
    save_QTimeEdit(group_name);
#endif
#ifdef SAVE_WIDGETS_PLAINTEXTEDIT
    save_QPlainTextEdit(group_name);
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
int MyWidget::load_int(QString name)
{
    return settings->value(name).toInt();
}
//--------------------------------------------------------------------------------
QString MyWidget::load_string(QString name)
{
    return settings->value(name).toString();
}
//--------------------------------------------------------------------------------
void MyWidget::save_int(QString name, int value)
{
    settings->setValue(name, value);
}
//--------------------------------------------------------------------------------
void MyWidget::save_string(QString name, QString value)
{
    settings->setValue(name, value);
}
//--------------------------------------------------------------------------------
QByteArray MyWidget::load_bytearray(QString name)
{
    return settings->value(name).toByteArray();
}
//--------------------------------------------------------------------------------
void MyWidget::save_bytearray(QString name, QByteArray value)
{
    settings->setValue(name, value);
}
//--------------------------------------------------------------------------------
QVariant MyWidget::load_value(QString name)
{
    return settings->value(name);
}
//--------------------------------------------------------------------------------
void MyWidget::save_value(QString name, QVariant value)
{
    settings->setValue(name, value);
}
//--------------------------------------------------------------------------------
void MyWidget::beginGroup(const QString &prefix)
{
    settings->beginGroup(prefix);
}
//--------------------------------------------------------------------------------
void MyWidget::endGroup(void)
{
    settings->endGroup();
}
//--------------------------------------------------------------------------------
void MyWidget::beginWriteArray(const QString &prefix, int size)
{
    settings->beginWriteArray(prefix, size);
}
//--------------------------------------------------------------------------------
int MyWidget::beginReadArray(const QString &prefix)
{
    return settings->beginReadArray(prefix);
}
//--------------------------------------------------------------------------------
void MyWidget::endArray(void)
{
    settings->endArray();
}
//--------------------------------------------------------------------------------
void MyWidget::setArrayIndex(int i)
{
    settings->setArrayIndex(i);
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
void MyWidget::debug(void)
{
    show_objectname();
    // block_wheel();
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
    QSpacerItem* horizontalSpacer = new QSpacerItem(static_cast<int>(width), 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = static_cast<QGridLayout *>(msgBox.layout());
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
    QSpacerItem* horizontalSpacer = new QSpacerItem(static_cast<int>(width), 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = static_cast<QGridLayout *>(msgBox.layout());
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    return msgBox.exec();
}
//--------------------------------------------------------------------------------
int MyWidget::messagebox_question(const QString title,
                                  const QString text,
                                  unsigned int width)
{
    QMessageBox msgBox;

    //msgBox.setIconPixmap(QPixmap(":/qmessagebox/qmessagebox-quest.png"));
    msgBox.setIcon(QMessageBox::Question);

    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    QSpacerItem* horizontalSpacer = new QSpacerItem(static_cast<int>(width), 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = static_cast<QGridLayout *>(msgBox.layout());
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
    QSpacerItem* horizontalSpacer = new QSpacerItem(static_cast<int>(width), 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = static_cast<QGridLayout *>(msgBox.layout());
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
    QSpacerItem* horizontalSpacer = new QSpacerItem(static_cast<int>(width), 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = static_cast<QGridLayout *>(msgBox.layout());
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
    Q_CHECK_PTR(settings);

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
    Q_CHECK_PTR(settings);

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
    Q_CHECK_PTR(settings);

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
    Q_CHECK_PTR(settings);

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
}
//--------------------------------------------------------------------------------
#include <QStringList>
#include <QPainter>
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
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
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
void MyWidget::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        updateText();
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
#ifndef BLOCK_WHEEL
bool MyWidget::eventFilter(QObject*, QEvent* event)
{
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    //---
    if(mouseEvent->type() == QMouseEvent::Wheel)
    {
        return true;
    }
    return false;
}
#endif
//--------------------------------------------------------------------------------
