/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#include <QtGlobal>

#ifdef Q_OS_LINUX
#   include </usr/include/linux/stddef.h>
#endif
//--------------------------------------------------------------------------------
#include "mysettings.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
MySettings::MySettings()
{
    init();
}
//--------------------------------------------------------------------------------
MySettings::~MySettings()
{
    if(settings)
    {
        delete settings;
    }
}
//--------------------------------------------------------------------------------
void MySettings::init(void)
{
#ifdef QT_DEBUG
    QString app_name = QString("%1(debug)").arg(APPNAME);
#else
    QString app_name = APPNAME;
#endif

#ifndef SAVE_INI
    settings = new QSettings(ORGNAME, app_name);
#else
    settings = new QSettings(QString("%1%2").arg(app_name).arg(".ini"), QSettings::IniFormat);
#endif
}
//--------------------------------------------------------------------------------
bool MySettings::load_combobox_property(QWidget *widget)
{
    Q_ASSERT(widget);
    Q_ASSERT(settings);

    if(compare_name(widget->metaObject()->className(), "QComboBox"))
    {
        int size = settings->beginReadArray(dynamic_cast<QComboBox *>(widget)->objectName());
        for(int n=0; n<size; n++)
        {
            settings->setArrayIndex(n);
            static_cast<QComboBox *>(widget)->addItem(settings->value("currentText").toString());
        }
        settings->endArray();
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
bool MySettings::save_combobox_property(QWidget *widget)
{
    Q_ASSERT(widget);
    Q_ASSERT(settings);

    if(compare_name(widget->metaObject()->className(), "QComboBox"))
    {
        settings->beginWriteArray(reinterpret_cast<QComboBox *>(widget)->objectName(), reinterpret_cast<QComboBox *>(widget)->count());
        int temp_index = reinterpret_cast<QComboBox *>(widget)->currentIndex();
        for(int n=0; n<static_cast<QComboBox *>(widget)->count(); n++)
        {
            settings->setArrayIndex(n);
            reinterpret_cast<QComboBox *>(widget)->setCurrentIndex(n);
            settings->setValue("currentText", QVariant::fromValue(reinterpret_cast<QComboBox *>(widget)->currentText()));
        }
        reinterpret_cast<QComboBox *>(widget)->setCurrentIndex(temp_index);
        settings->endArray();
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
bool MySettings::load_listwidget_property(QWidget *widget)
{
    Q_ASSERT(widget);
    Q_ASSERT(settings);

    QListWidget *lw = dynamic_cast<QListWidget *>(widget);
    if(lw)
    {
        settings->beginGroup(get_full_objectName(widget));
        int size = settings->beginReadArray(static_cast<QListWidget *>(widget)->objectName());
        for(int n=0; n<size; n++)
        {
            settings->setArrayIndex(n);
            reinterpret_cast<QListWidget *>(widget)->addItem(settings->value("currentText").toString());
        }
        settings->endArray();
        settings->endGroup();
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
bool MySettings::save_listwidget_property(QWidget *widget)
{
    Q_ASSERT(widget);
    Q_ASSERT(settings);

    QListWidget *lw = dynamic_cast<QListWidget *>(widget);
    if(lw)
    {
        settings->beginGroup(get_full_objectName(widget));
        settings->beginWriteArray(reinterpret_cast<QListWidget *>(widget)->objectName(), reinterpret_cast<QListWidget *>(widget)->count());
        for(int n=0; n<reinterpret_cast<QListWidget *>(widget)->count(); n++)
        {
            settings->setArrayIndex(n);
            reinterpret_cast<QListWidget *>(widget)->setCurrentRow(n);
            settings->setValue("currentText", QVariant::fromValue(reinterpret_cast<QListWidget *>(widget)->currentItem()->text()));
        }
        settings->endArray();
        settings->endGroup();
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
bool MySettings::load_splitter_property(QWidget *widget)
{
    Q_ASSERT(widget);

    if(compare_name(widget->metaObject()->className(), "QSplitter"))
    {
        QString o_name = widget->objectName();
        if(o_name.isEmpty() == false)
        {
            QByteArray ba = load_bytearray(o_name);
            if(ba.isEmpty() == false)
            {
                static_cast<QSplitter *>(widget)->restoreState(ba);
                return true;
            }
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
bool MySettings::save_splitter_property(QWidget *widget)
{
    Q_ASSERT(widget);

    if(compare_name(widget->metaObject()->className(), "QSplitter"))
    {
        QString o_name = widget->objectName();
        if(o_name == "main_splitter")
        {
#ifdef QT_DEBUG
            qDebug() << "found splitter!";
#endif
        }
        if(o_name.isEmpty() == false)
        {
            QByteArray ba = static_cast<QSplitter *>(widget)->saveState();
            if(ba.isEmpty() == false)
            {
                save_bytearray(o_name, ba);
                return true;
            }
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
bool MySettings::load_mdi_subwindows(QWidget *widget)
{
    Q_ASSERT(widget);

    if(compare_name(widget->metaObject()->className(), "QMdiSubWindow"))
    {
        if(widget->objectName().isEmpty() == false)
        {
            //qDebug() << "### found" << widget->objectName();
            int x = 0;
            int y = 0;
            int w = 0;
            int h = 0;
            bool ok;
            ok = load_int("pos_x", &x);
            if(!ok) return false;
            ok = load_int("pos_y", &y);
            if(!ok) return false;
            ok = load_int("widget_w", &w);
            if(!ok) return false;
            ok = load_int("widget_h", &h);
            if(!ok) return false;

            widget->resize(w, h);
            widget->move(x, y);
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
bool MySettings::save_mdi_subwindows(QWidget *widget)
{
    Q_ASSERT(widget);

    if(compare_name(widget->metaObject()->className(), "QMdiSubWindow"))
    {
        if(widget->objectName().isEmpty() == false)
        {
            //qDebug() << "### found" << widget->objectName();
            save_int("pos_x", widget->x());
            save_int("pos_y", widget->y());
            save_int("widget_w", widget->width());
            save_int("widget_h", widget->height());
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
bool MySettings::load_property(QWidget *widget, const QString &property_name)
{
    Q_ASSERT(widget);

    //TODO временный костыль
    QLabel *label = dynamic_cast<QLabel *>(widget);
    if(label)
    {
        return false;
    }
    //---

    QVariant property = settings->value(property_name);
    if(property.isValid() == false)
    {
        return false;
    }
    if(widget->property(NO_SAVE).toBool())
    {
        return false;
    }
    bool ok = widget->setProperty(property_name.toLocal8Bit(), property);
    return ok;
}
//--------------------------------------------------------------------------------
bool MySettings::save_property(QWidget *widget, const QString &property_name)
{
    Q_ASSERT(widget);

    //TODO временный костыль
    QLabel *label = dynamic_cast<QLabel *>(widget);
    if(label)
    {
        return false;
    }
    //---

    QVariant property = widget->property(property_name.toLocal8Bit());
    if(property.isValid() == false)
    {
        return false;
    }
    if(widget->property(NO_SAVE).toBool())
    {
        return false;
    }
    settings->setValue(property_name, QVariant::fromValue(property));
    return true;
}
//--------------------------------------------------------------------------------
bool MySettings::load_int(const QString &name, int *value)
{
    Q_ASSERT(settings);

    bool ok = false;
    int res = settings->value(name).toInt(&ok);
    if(ok == false)
    {
        return false;
    }
    *value = res;
    return true;
}
//--------------------------------------------------------------------------------
void MySettings::save_int(const QString &name, int value)
{
    Q_ASSERT(settings);
    settings->setValue(name, QVariant::fromValue(value));
}
//--------------------------------------------------------------------------------
bool MySettings::load_ulonglong(const QString &name, qlonglong *value)
{
    Q_ASSERT(settings);

    bool ok = false;
    int res = settings->value(name).toULongLong(&ok);
    if(ok == false)
    {
        return false;
    }
    *value = res;
    return true;
}
//--------------------------------------------------------------------------------
void MySettings::save_ulonglong(const QString &name, qlonglong value)
{
    Q_ASSERT(settings);
    settings->setValue(name, QVariant::fromValue(value));
}
//--------------------------------------------------------------------------------
bool MySettings::load_bool(const QString &name, QVariant default_value)
{
    Q_ASSERT(settings);
    return settings->value(name, default_value).toBool();
}
//--------------------------------------------------------------------------------
void MySettings::save_bool(const QString &name, bool value)
{
    Q_ASSERT(settings);
    settings->setValue(name, QVariant::fromValue(value));
}
//--------------------------------------------------------------------------------
bool MySettings::load_uint(const QString &name, uint *value)
{
    Q_ASSERT(settings);

    bool ok = false;
    uint res = settings->value(name).toUInt(&ok);
    if(ok == false)
    {
        return false;
    }
    *value = res;
    return true;
}
//--------------------------------------------------------------------------------
void MySettings::save_uint(const QString &name, uint value)
{
    Q_ASSERT(settings);
    settings->setValue(name, QVariant::fromValue(value));
}
//--------------------------------------------------------------------------------
QString MySettings::load_string(const QString &name)
{
    Q_ASSERT(settings);
    return settings->value(name).toString();
}
//--------------------------------------------------------------------------------
void MySettings::save_string(const QString &name, QString value)
{
    Q_ASSERT(settings);
    settings->setValue(name, QVariant::fromValue(value));
}
//--------------------------------------------------------------------------------
QByteArray MySettings::load_bytearray(const QString &name)
{
    Q_ASSERT(settings);
    QByteArray ba;
    ba = settings->value(name).toByteArray();
    return ba;
}
//--------------------------------------------------------------------------------
void MySettings::save_bytearray(const QString &name, QByteArray value)
{
    Q_ASSERT(settings);
    settings->setValue(name, QVariant::fromValue(value));
}
//--------------------------------------------------------------------------------
QStringList MySettings::load_stringlist(const QString &name)
{
    Q_ASSERT(settings);
    QStringList sl;
    sl = settings->value(name).toStringList();
    return sl;
}
//--------------------------------------------------------------------------------
void MySettings::save_stringlist(const QString &name, QStringList value)
{
    Q_ASSERT(settings);
    settings->setValue(name, QVariant::fromValue(value));
}
//--------------------------------------------------------------------------------
QVariant MySettings::load_value(const QString &name, const QVariant &defaultValue)
{
    Q_ASSERT(settings);
    return settings->value(name, defaultValue);
}
//--------------------------------------------------------------------------------
void MySettings::save_value(const QString &name, QVariant value)
{
    Q_ASSERT(settings);
    settings->setValue(name, value);
}
//--------------------------------------------------------------------------------
void MySettings::beginGroup(const QString &prefix)
{
    Q_ASSERT(settings);
    settings->beginGroup(prefix);
}
//--------------------------------------------------------------------------------
void MySettings::endGroup(void)
{
    Q_ASSERT(settings);
    settings->endGroup();
}
//--------------------------------------------------------------------------------
void MySettings::beginWriteArray(const QString &prefix, int size)
{
    Q_ASSERT(settings);
    settings->beginWriteArray(prefix, size);
}
//--------------------------------------------------------------------------------
int MySettings::beginReadArray(const QString &prefix)
{
    Q_ASSERT(settings);
    return settings->beginReadArray(prefix);
}
//--------------------------------------------------------------------------------
void MySettings::endArray(void)
{
    Q_ASSERT(settings);
    settings->endArray();
}
//--------------------------------------------------------------------------------
void MySettings::setArrayIndex(int i)
{
    Q_ASSERT(settings);
    settings->setArrayIndex(i);
}
//--------------------------------------------------------------------------------
QString MySettings::get_full_objectName(QWidget *widget)
{
    Q_ASSERT(widget);

    QStringList sl;
    QWidget *temp = reinterpret_cast<QWidget *>(widget);
    do {
        if(temp)
        {
            sl.append(temp->objectName());
        }
        temp = reinterpret_cast<QWidget *>(temp->parent());
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
bool MySettings::is_my_widget(const QString &o_name)
{
    if(o_name.isEmpty())
    {
        return false;
    }

    return (o_name.left(3) != "qt_");
}
//--------------------------------------------------------------------------------
bool MySettings::compare_name(const char *widget_name, QString class_name)
{
    Q_ASSERT(widget_name);
    int res = strncmp(widget_name,
                      class_name.toLocal8Bit(),
                      static_cast<size_t>(class_name.count()));
    return (res == 0);
}
//--------------------------------------------------------------------------------
void MySettings::clear_all(void)
{
    Q_ASSERT(settings);
    settings->clear();
}
//--------------------------------------------------------------------------------
