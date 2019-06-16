/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "labyrinth_mainbox.hpp"
#include "defines.hpp"
#include "mouse.hpp"
//--------------------------------------------------------------------------------
#include "ui_mouse.h"
//--------------------------------------------------------------------------------
Mouse::Mouse(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Mouse)
{
    init();
}
//--------------------------------------------------------------------------------
Mouse::~Mouse()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Mouse::init(void)
{
    ui->setupUi(this);

    bool ok = false;
    ok = ui->w_minimap->new_map(3,3);
    if(!ok)
    {
        emit error("ui->w_minimap->new_map(3,3)");
    }

    for(int y=0; y<3; y++)
    {
        for(int x=0; x<3; x++)
        {
            ok = ui->w_minimap->set(x, y, SPACE_ID);
            if(!ok)
            {
                emit error(QString("ui->w_minimap->set(%1, %2, SPACE_ID)").arg(x).arg(y));
            }
        }
    }
}
//--------------------------------------------------------------------------------
bool Mouse::set(int x, int y, int id)
{
    return ui->w_minimap->set(x, y, id);
}
//--------------------------------------------------------------------------------
bool Mouse::get(int x, int y, int *id)
{
    return ui->w_minimap->get(x, y, id);
}
//--------------------------------------------------------------------------------
void Mouse::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool Mouse::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Mouse::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void Mouse::save_setting(void)
{

}
//--------------------------------------------------------------------------------
