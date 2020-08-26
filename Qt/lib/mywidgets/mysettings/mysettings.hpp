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
#ifndef MYSETTINGS_HPP
#define MYSETTINGS_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include<QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
class MySettings
{
public:
    MySettings();
    ~MySettings();

    bool is_my_widget(QString o_name);
    bool compare_name(const char *widget_name, QString class_name);

    bool load_combobox_property(QWidget *widget);
    bool save_combobox_property(QWidget *widget);

    bool load_listwidget_property(QWidget *widget);
    bool save_listwidget_property(QWidget *widget);

    bool load_splitter_property(QWidget *widget);
    bool save_splitter_property(QWidget *widget);

    bool load_property(QWidget *widget, const QString &property_name);
    bool save_property(QWidget *widget, const QString &property_name);

    bool load_int(QString name, int *value);
    void save_int(QString name, int value);

    bool load_ulonglong(QString name, qlonglong *value);
    void save_ulonglong(QString name, qlonglong value);

    bool load_bool(QString name);
    void save_bool(QString name, bool value);

    bool load_uint(QString name, uint *value);
    void save_uint(QString name, uint value);

    QString load_string(QString name);
    void save_string(QString name, QString value);

    QByteArray load_bytearray(QString name);
    void save_bytearray(QString name, QByteArray value);

    QStringList load_stringlist(QString name);
    void save_stringlist(QString name, QStringList value);

    QVariant load_value(QString name, const QVariant &defaultValue = QVariant());
    void save_value(QString name, QVariant value);

    void beginGroup(const QString &prefix);
    void endGroup(void);

    void beginWriteArray(const QString &prefix, int size = -1);
    int  beginReadArray(const QString &prefix);

    void endArray(void);
    void setArrayIndex(int i);

private:
    QPointer<QSettings> settings;

    void init(void);
    QString get_full_objectName(QWidget *widget);
};
//--------------------------------------------------------------------------------
#endif
