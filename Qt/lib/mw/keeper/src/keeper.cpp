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
#include "keeper.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
Keeper::Keeper(QWidget *parent) :
    QWidget(parent)
{

}
//--------------------------------------------------------------------------------
Keeper::~Keeper()
{

}
//--------------------------------------------------------------------------------
bool Keeper::set_value(QString name, QVariant value)
{
#ifndef SAVE_INI
    QSettings *settings = new QSettings(ORGNAME, APPNAME);
#else
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
#endif
    settings->setValue(name, value);
    settings->deleteLater();
    return true;
}
//--------------------------------------------------------------------------------
bool Keeper::get_value(QString name, QVariant *value)
{
#ifndef SAVE_INI
    QSettings *settings = new QSettings(ORGNAME, APPNAME);
#else
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
#endif
    *value = settings->value(name);
    settings->deleteLater();
    return true;
}
//--------------------------------------------------------------------------------
bool Keeper::set_array(QString group_name,
                       QString name,
                       QList<QVariant> l_values)
{
#ifndef SAVE_INI
    QSettings *settings = new QSettings(ORGNAME, APPNAME);
#else
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
#endif
    settings->beginGroup(group_name);
    settings->beginWriteArray(name, l_values.count());
    for(int n=0; n<l_values.count(); n++)
    {
        settings->setArrayIndex(n);
        settings->setValue("value", l_values.at(n));
    }
    settings->endGroup();

    settings->deleteLater();
    return true;
}
//--------------------------------------------------------------------------------
bool Keeper::get_array(QString group_name,
                       QString name,
                       QList<QVariant> *l_values)
{
#ifndef SAVE_INI
    QSettings *settings = new QSettings(ORGNAME, APPNAME);
#else
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
#endif

    return true;
}
//--------------------------------------------------------------------------------
