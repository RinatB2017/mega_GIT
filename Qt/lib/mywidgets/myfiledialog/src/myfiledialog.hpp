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
#ifndef MYFILEDIALOG_HPP
#define MYFILEDIALOG_HPP
//--------------------------------------------------------------------------------
#include <QFileDialog>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "mysettings.hpp"
//--------------------------------------------------------------------------------
class MyFileDialog : public QFileDialog, public MySettings
{
    Q_OBJECT

public:
    explicit MyFileDialog(const QString &gName,
                          const QString &oName,
                          QWidget *parent = nullptr);
    virtual ~MyFileDialog();

public slots:
    int exec(void);

private:
    QString gName;
    QString oName;
    QString dir;
};
//--------------------------------------------------------------------------------
#endif // MYFILEDIALOG_HPP
