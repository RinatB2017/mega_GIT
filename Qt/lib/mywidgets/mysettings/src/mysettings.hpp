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
#include <QListWidget>
#include <QSplitter>
#include <QSettings>
#include <QPointer>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
#define PARAMS_GROUP_NAME   "Params"
#define NO_BLOCK    "no_block"
#define NO_SAVE     "no_save"

#define STATE_PROPERTY  "state"
#define FLAG_NO_CLOSE   "flag_no_close"
//--------------------------------------------------------------------------------
class MySettings
{
public:
    MySettings();
    virtual ~MySettings();

    bool is_my_widget(const QString &o_name);
    bool compare_name(const char *widget_name, QString class_name);

    bool load_combobox_property(QWidget *widget);
    bool save_combobox_property(QWidget *widget);

    bool load_listwidget_property(QWidget *widget);
    bool save_listwidget_property(QWidget *widget);

    bool load_splitter_property(QWidget *widget);
    bool save_splitter_property(QWidget *widget);

    bool load_mdi_subwindows(QWidget *widget);
    bool save_mdi_subwindows(QWidget *widget);

    bool load_property(QWidget *widget, const QString &property_name);
    bool save_property(QWidget *widget, const QString &property_name);

    bool load_int(const QString &name, int *value);
    void save_int(const QString &name, int value);

    bool load_ulonglong(const QString &name, qlonglong *value);
    void save_ulonglong(const QString &name, qlonglong value);

    bool load_bool(const QString &name, QVariant default_value = false);
    void save_bool(const QString &name, bool value);

    bool load_uint(const QString &name, uint *value);
    void save_uint(const QString &name, uint value);

    QString load_string(const QString &name);
    void save_string(const QString &name, QString value);

    QByteArray load_bytearray(const QString &name);
    void save_bytearray(const QString &name, QByteArray value);

    QStringList load_stringlist(const QString &name);
    void save_stringlist(const QString &name, QStringList value);

    QVariant load_value(const QString &name, const QVariant &defaultValue = QVariant());
    void save_value(const QString &name, QVariant value);

    void beginGroup(const QString &prefix);
    void endGroup(void);

    void beginWriteArray(const QString &prefix, int size = -1);
    int  beginReadArray(const QString &prefix);

    QString get_full_objectName(QWidget *widget);

    void endArray(void);
    void setArrayIndex(int i);

    void clear_all(void);

private:
    QPointer<QSettings> settings;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif
