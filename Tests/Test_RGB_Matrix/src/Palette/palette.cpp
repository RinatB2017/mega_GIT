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
#include <QGridLayout>
#include <QToolButton>
#include <QSettings>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "palette.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "diod.hpp"
//--------------------------------------------------------------------------------
MyPalette::MyPalette(int max_x,
                     int max_y,
                     QWidget *parent) :
    QGroupBox(parent)
{
    grid = new QGridLayout();
    grid->setMargin(0);
    grid->setSpacing(0);
    for(int y=0; y<max_y; y++)
    {
        for(int x=0; x<max_x; x++)
        {
            Diod *diod = new Diod(this);
            diod->setProperty(PALETTE_PROPERTY, flag_active);
            buttons.append(diod);

            grid->addWidget(diod, y, x);
        }
    }
    QVBoxLayout *box = new QVBoxLayout;
    box->addLayout(grid);
    box->addStretch(1);

    setLayout(box);

    load_setting();
}
//--------------------------------------------------------------------------------
MyPalette::~MyPalette()
{
    save_setting();
}
//--------------------------------------------------------------------------------
void MyPalette::set_active(bool value)
{
    flag_active = value;
    foreach (Diod *diod, buttons)
    {
        diod->setProperty(PALETTE_PROPERTY, value);
    }
}
//--------------------------------------------------------------------------------
void MyPalette::load_setting(void)
{
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    Q_CHECK_PTR(settings);

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyPalette::save_setting(void)
{
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    Q_CHECK_PTR(settings);

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
