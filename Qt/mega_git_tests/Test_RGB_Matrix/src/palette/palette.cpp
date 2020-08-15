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
//--------------------------------------------------------------------------------
#include "palette.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "diod.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MyPalette::MyPalette(int max_x,
                     int max_y,
                     QWidget *parent) :
    QGroupBox(parent)
{
    this->max_x = max_x;
    this->max_y = max_y;

    grid = new QGridLayout();
    grid->setMargin(0);
    grid->setSpacing(0);
    for(int y=0; y<max_y; y++)
    {
        for(int x=0; x<max_x; x++)
        {
            Diod *diod = new Diod(32, 32, this);
            diod->set_left_btn_active(true);
            diod->set_right_btn_active(true);
            diod->set_flag_is_palette(true);
            buttons.append(diod);

            grid->addWidget(diod, y, x);
        }
    }
    QVBoxLayout *box = new QVBoxLayout();
    box->addLayout(grid);
    box->addStretch(1);

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    setLayout(box);
}
//--------------------------------------------------------------------------------
MyPalette::~MyPalette()
{
    save_setting();
}
//--------------------------------------------------------------------------------
void MyPalette::set_left_btn_active(bool value)
{
    flag_active = value;
    foreach (Diod *diod, buttons)
    {
        diod->set_left_btn_active(value);
    }
}
//--------------------------------------------------------------------------------
void MyPalette::set_right_btn_active(bool value)
{
    flag_active = value;
    foreach (Diod *diod, buttons)
    {
        diod->set_right_btn_active(value);
    }
}
//--------------------------------------------------------------------------------
void MyPalette::set_flag_is_palette(bool value)
{
    flag_active = value;
    foreach (Diod *diod, buttons)
    {
        diod->set_flag_is_palette(value);
    }
}
//--------------------------------------------------------------------------------
void MyPalette::set_data(QByteArray data)
{
    if(data.length() != (max_x * max_y * 3))
    {
#ifdef QT_DEBUG
        qDebug() << "MyPalette::set_data:" << data.length() << (max_x * max_y * 3);
#endif
        emit error("MyPalette::set_data bad data size!");
        return;
    }

    int index = 0;
    foreach (Diod *diod, buttons)
    {
        diod->set_color(static_cast<uint8_t>(data[index]),
                        static_cast<uint8_t>(data[index+1]),
                        static_cast<uint8_t>(data[index+2]));
        index+=3;
    }
}
//--------------------------------------------------------------------------------
QByteArray MyPalette::get_data(void)
{
    QByteArray ba;

    foreach (Diod *diod, buttons)
    {
        ba.append(static_cast<char>(diod->get_R()));
        ba.append(static_cast<char>(diod->get_G()));
        ba.append(static_cast<char>(diod->get_B()));
    }
    return ba;
}
//--------------------------------------------------------------------------------
void MyPalette::load_setting(void)
{
#ifndef SAVE_INI
    QSettings *settings = new QSettings(ORGNAME, APPNAME);
#else
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
#endif
    Q_CHECK_PTR(settings);

    settings->beginGroup(objectName());
    set_data(settings->value("value").toByteArray());
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyPalette::save_setting(void)
{
#ifndef SAVE_INI
    QSettings *settings = new QSettings(ORGNAME, APPNAME);
#else
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
#endif
    Q_CHECK_PTR(settings);

    settings->beginGroup(objectName());
    settings->setValue("value", get_data());
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
