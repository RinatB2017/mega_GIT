/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#include <QSettings>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "myfiledialog.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
MyFileDialog::MyFileDialog(const QString gName,
                           const QString oName,
                           QWidget *parent) :
    QFileDialog(parent),
    gName(gName),
    oName(oName)
{
    setOption(MyFileDialog::DontUseNativeDialog, false);
    setObjectName(oName);
    //---
#ifndef SAVE_INI
    settings = new QSettings(ORGNAME, APPNAME);
#else
    settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
#endif
    if(settings)
    {
        settings->beginGroup(gName);

        QString dir = settings->value(oName, ".").toString();
        setDirectory(dir);

        settings->endGroup();
        settings->deleteLater();
    }
    //---
}
//--------------------------------------------------------------------------------
int MyFileDialog::exec(void)
{
    int res = QFileDialog::exec();

    if(res == Accepted)
    {
#ifndef SAVE_INI
        settings = new QSettings(ORGNAME, APPNAME);
#else
        settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
#endif
        if(settings)
        {
            settings->beginGroup(gName);

            settings->setValue(oName, directory().absolutePath());

            settings->endGroup();
            settings->deleteLater();
        }
    }

    return res;
}
//--------------------------------------------------------------------------------
