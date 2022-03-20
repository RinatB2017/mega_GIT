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
Palette::Palette(QWidget *parent) :
    QGroupBox(parent)
{
    //set_param(4, 4);
    //load_setting();
}
//--------------------------------------------------------------------------------
Palette::~Palette()
{
    save_setting();
}
//--------------------------------------------------------------------------------
void Palette::set_left_btn_active(bool value)
{
    flag_active = value;
    for(int y=0; y<max_y; y++)
    {
        for(int x=0; x<max_x; x++)
        {
            a_diod[x][y]->set_left_btn_active(value);
        }
    }
}
//--------------------------------------------------------------------------------
void Palette::set_right_btn_active(bool value)
{
    flag_active = value;
    for(int y=0; y<max_y; y++)
    {
        for(int x=0; x<max_x; x++)
        {
            a_diod[x][y]->set_right_btn_active(value);
        }
    }
}
//--------------------------------------------------------------------------------
void Palette::set_flag_is_palette(bool value)
{
    flag_active = value;
    for(int y=0; y<max_y; y++)
    {
        for(int x=0; x<max_x; x++)
        {
            a_diod[x][y]->set_flag_is_palette(value);
        }
    }
}
//--------------------------------------------------------------------------------
void Palette::set_data(QByteArray data)
{
    if(data.length() != (max_x * max_y * 3))
    {
        emit error("Palette::set_data bad data size!");
        return;
    }

    int index=0;
    for(int y=0; y<max_y; y++)
    {
        for(int x=0; x<max_x; x++)
        {
            a_diod[x][y]->set_color(static_cast<uint8_t>(data[index]),
                                    static_cast<uint8_t>(data[index+1]),
                    static_cast<uint8_t>(data[index+2]));
            index+=3;
        }
    }
}
//--------------------------------------------------------------------------------
void Palette::set_param(int size_x,
                        int size_y,
                        int led_w,
                        int led_h)
{
    max_x = size_x;
    max_y = size_y;
    led_width = led_w;
    led_height = led_h;
    if(grid)    delete grid;

    grid = new QGridLayout();
    grid->setMargin(0);
    grid->setSpacing(0);
    for(int y=0; y<max_y; y++)
    {
        for(int x=0; x<max_x; x++)
        {
            a_diod[x][y] = new Diod(led_width, led_height, this);
            a_diod[x][y]->set_left_btn_active(true);
            a_diod[x][y]->set_right_btn_active(true);
            a_diod[x][y]->set_flag_is_palette(true);

            grid->addWidget(a_diod[x][y], y, x);
        }
    }
    QVBoxLayout *box = new QVBoxLayout();
    box->addLayout(grid);
    box->addStretch(1);

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    setLayout(box);

    load_setting();
}
//--------------------------------------------------------------------------------
QByteArray Palette::get_data(void)
{
    uint8_t R = 0;
    uint8_t G = 0;
    uint8_t B = 0;
    QByteArray ba;

    for(int y=0; y<max_y; y++)
    {
        for(int x=0; x<max_x; x++)
        {
            R = a_diod[x][y]->get_R();
            G = a_diod[x][y]->get_G();
            B = a_diod[x][y]->get_B();

            ba.append(static_cast<char>(R));
            ba.append(static_cast<char>(G));
            ba.append(static_cast<char>(B));
        }
    }
    return ba;
}
//--------------------------------------------------------------------------------
void Palette::load_setting(void)
{
#ifndef SAVE_INI
    QSettings *settings = new QSettings(ORGNAME, APPNAME);
#else
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
#endif
    Q_ASSERT(settings);

    settings->beginGroup(objectName());
    set_data(settings->value("value").toByteArray());
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void Palette::save_setting(void)
{
#ifndef SAVE_INI
    QSettings *settings = new QSettings(ORGNAME, APPNAME);
#else
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
#endif
    Q_ASSERT(settings);

    settings->beginGroup(objectName());
    settings->setValue("value", get_data());
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
