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
        settings->deleteLater();
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
#if 0
bool MySettings::load_combobox_property(QWidget *widget)
{
    Q_CHECK_PTR(widget);
    if(compare_name(widget->metaObject()->className(), "QComboBox"))
    {
        bool isEditable = settings->value("isEditable").toBool();
        if(isEditable)
        {
            int size = settings->beginReadArray(static_cast<QComboBox *>(widget)->objectName());
            for(int n=0; n<size; n++)
            {
                settings->setArrayIndex(n);
                static_cast<QComboBox *>(widget)->addItem(settings->value("currentText").toString());
            }
            settings->endArray();
        }
        static_cast<QComboBox *>(widget)->setCurrentIndex(settings->value("currentindex", 0).toInt());
        return true;
    }
    return false;
}
#else
bool MySettings::load_combobox_property(QWidget *widget)
{
    Q_CHECK_PTR(widget);
    if(compare_name(widget->metaObject()->className(), "QComboBox"))
    {
        int size = settings->beginReadArray(static_cast<QComboBox *>(widget)->objectName());
        for(int n=0; n<size; n++)
        {
            settings->setArrayIndex(n);
            static_cast<QComboBox *>(widget)->addItem(settings->value("currentText").toString());
        }
        settings->endArray();
        //static_cast<QComboBox *>(widget)->setCurrentIndex(settings->value("currentindex", 0).toInt());  //TODO проба
        return true;
    }
    return false;
}
#endif
//--------------------------------------------------------------------------------
#if 0
bool MySettings::save_combobox_property(QWidget *widget)
{
    Q_CHECK_PTR(widget);
    if(compare_name(widget->metaObject()->className(), "QComboBox"))
    {
        bool isEditable = static_cast<QComboBox *>(widget)->isEditable();
        settings->setValue("isEditable", isEditable);
        settings->setValue("currentindex", QVariant(static_cast<QComboBox *>(widget)->currentIndex()));
        if(isEditable)
        {
            settings->beginWriteArray(static_cast<QComboBox *>(widget)->objectName(), static_cast<QComboBox *>(widget)->count());
            for(int n=0; n<static_cast<QComboBox *>(widget)->count(); n++)
            {
                settings->setArrayIndex(n);
                static_cast<QComboBox *>(widget)->setCurrentIndex(n);
                settings->setValue("currentText", static_cast<QComboBox *>(widget)->currentText());
            }
            settings->endArray();
        }
        return true;
    }
    return false;
}
#else
bool MySettings::save_combobox_property(QWidget *widget)
{
    Q_CHECK_PTR(widget);
    if(compare_name(widget->metaObject()->className(), "QComboBox"))
    {
        //settings->setValue("currentindex", QVariant(static_cast<QComboBox *>(widget)->currentIndex()));   //TODO проба
        settings->beginWriteArray(static_cast<QComboBox *>(widget)->objectName(), static_cast<QComboBox *>(widget)->count());
        for(int n=0; n<static_cast<QComboBox *>(widget)->count(); n++)
        {
            settings->setArrayIndex(n);
            static_cast<QComboBox *>(widget)->setCurrentIndex(n);
            settings->setValue("currentText", static_cast<QComboBox *>(widget)->currentText());
        }
        settings->endArray();
        return true;
    }
    return false;
}
#endif
//--------------------------------------------------------------------------------
bool MySettings::load_listwidget_property(QWidget *widget)
{
    Q_CHECK_PTR(widget);

    QListWidget *lw = dynamic_cast<QListWidget *>(widget);
    if(lw)
    {
        settings->beginGroup(get_full_objectName(widget));
        int size = settings->beginReadArray(static_cast<QListWidget *>(widget)->objectName());
        for(int n=0; n<size; n++)
        {
            settings->setArrayIndex(n);
            dynamic_cast<QListWidget *>(widget)->addItem(settings->value("currentText").toString());
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
    Q_CHECK_PTR(widget);
    QListWidget *lw = dynamic_cast<QListWidget *>(widget);
    if(lw)
    {
        settings->beginGroup(get_full_objectName(widget));
        //settings->setValue("currentindex", QVariant(dynamic_cast<QListWidget *>(widget)->currentIndex()));    //TODO проба
        settings->beginWriteArray(dynamic_cast<QListWidget *>(widget)->objectName(), dynamic_cast<QListWidget *>(widget)->count());
        for(int n=0; n<dynamic_cast<QListWidget *>(widget)->count(); n++)
        {
            settings->setArrayIndex(n);
            dynamic_cast<QListWidget *>(widget)->setCurrentRow(n);
            settings->setValue("currentText", dynamic_cast<QListWidget *>(widget)->currentItem()->text());
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
    Q_CHECK_PTR(widget);
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
    Q_CHECK_PTR(widget);
    if(compare_name(widget->metaObject()->className(), "QSplitter"))
    {
        QString o_name = widget->objectName();
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
bool MySettings::load_property(QWidget *widget, const QString &property_name)
{
    Q_CHECK_PTR(widget);
    QVariant property = settings->value(property_name);
    if(property.isValid() == false)
    {
        return false;
    }
    bool ok = widget->setProperty(property_name.toLocal8Bit(), property);
    return ok;
}
//--------------------------------------------------------------------------------
bool MySettings::save_property(QWidget *widget, const QString &property_name)
{
    Q_CHECK_PTR(widget);
    QVariant property = widget->property(property_name.toLocal8Bit());
    if(property.isValid() == false)
    {
        return false;
    }
    settings->setValue(property_name, property);
    return true;
}
//--------------------------------------------------------------------------------
int MySettings::load_int(QString name)
{
    return settings->value(name).toInt();
}
//--------------------------------------------------------------------------------
void MySettings::save_int(QString name, int value)
{
    settings->setValue(name, value);
}
//--------------------------------------------------------------------------------
bool MySettings::load_bool(QString name)
{
    return settings->value(name).toBool();
}
//--------------------------------------------------------------------------------
void MySettings::save_bool(QString name, bool value)
{
    settings->setValue(name, value);
}
//--------------------------------------------------------------------------------
uint MySettings::load_uint(QString name)
{
    return settings->value(name).toUInt();
}
//--------------------------------------------------------------------------------
void MySettings::save_uint(QString name, uint value)
{
    settings->setValue(name, value);
}
//--------------------------------------------------------------------------------
QString MySettings::load_string(QString name)
{
    return settings->value(name).toString();
}
//--------------------------------------------------------------------------------
void MySettings::save_string(QString name, QString value)
{
    settings->setValue(name, value);
}
//--------------------------------------------------------------------------------
QByteArray MySettings::load_bytearray(QString name)
{
    QByteArray ba;
    if(settings)
    {
        ba = settings->value(name).toByteArray();
    }
    else
    {
        qDebug() << "### load_bytearray ###";
    }
    return ba;
}
//--------------------------------------------------------------------------------
void MySettings::save_bytearray(QString name, QByteArray value)
{
    if(settings)
    {
        settings->setValue(name, value);
    }
    else
    {
        qDebug() << "### save_bytearray ###";
    }
}
//--------------------------------------------------------------------------------
QStringList MySettings::load_stringlist(QString name)
{
    QStringList sl;
    if(settings)
    {
        sl = settings->value(name).toStringList();
    }
    else
    {
        qDebug() << "### load_stringlist ###";
    }
    return sl;
}
//--------------------------------------------------------------------------------
void MySettings::save_stringlist(QString name, QStringList value)
{
    if(settings)
    {
        settings->setValue(name, value);
    }
    else
    {
        qDebug() << "### save_stringlist ###";
    }
}
//--------------------------------------------------------------------------------
QVariant MySettings::load_value(QString name, const QVariant &defaultValue)
{
    return settings->value(name, defaultValue);
}
//--------------------------------------------------------------------------------
void MySettings::save_value(QString name, QVariant value)
{
    settings->setValue(name, value);
}
//--------------------------------------------------------------------------------
void MySettings::beginGroup(const QString &prefix)
{
    settings->beginGroup(prefix);
}
//--------------------------------------------------------------------------------
void MySettings::endGroup(void)
{
    settings->endGroup();
}
//--------------------------------------------------------------------------------
void MySettings::beginWriteArray(const QString &prefix, int size)
{
    settings->beginWriteArray(prefix, size);
}
//--------------------------------------------------------------------------------
int MySettings::beginReadArray(const QString &prefix)
{
    Q_CHECK_PTR(settings);
    return settings->beginReadArray(prefix);
}
//--------------------------------------------------------------------------------
void MySettings::endArray(void)
{
    settings->endArray();
}
//--------------------------------------------------------------------------------
void MySettings::setArrayIndex(int i)
{
    settings->setArrayIndex(i);
}
//--------------------------------------------------------------------------------
QString MySettings::get_full_objectName(QWidget *widget)
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
bool MySettings::is_my_widget(QString o_name)
{
    if(o_name.isEmpty())
        return false;

    return (o_name.left(3) != "qt_");
}
//--------------------------------------------------------------------------------
bool MySettings::compare_name(const char *widget_name, QString class_name)
{
    Q_CHECK_PTR(widget_name);
    int res = strncmp(widget_name,
                      class_name.toLocal8Bit(),
                      static_cast<size_t>(class_name.count()));
    return (res == 0);
}
//--------------------------------------------------------------------------------
