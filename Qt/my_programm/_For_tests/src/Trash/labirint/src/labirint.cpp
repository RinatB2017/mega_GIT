/*********************************************************************************
**                                                                              **
**     Copyright (C) 2019                                                       **
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
#include "labirint.hpp"
#include "ui_labirint.h"
//--------------------------------------------------------------------------------
Labirint::Labirint(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Labirint)
{
    init();
}
//--------------------------------------------------------------------------------
Labirint::~Labirint()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Labirint::init(void)
{
    ui->setupUi(this);

    connect(ui->w_minimap,  SIGNAL(cell_click(uint8_t)),
            ui->w_world,    SLOT(cell_click(uint8_t)));
    connect(ui->btn_load,   SIGNAL(clicked(bool)),
            this,           SLOT(load()));
    connect(ui->btn_save,   SIGNAL(clicked(bool)),
            this,           SLOT(save()));

    setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
void Labirint::set_cursor(void)
{
    emit info("set_cursor");
#if 0
    QToolButton *btn = dynamic_cast<QToolButton*>(sender());
    if(!btn)
    {
        emit error("set_cursor");
        return;
    }

    id = btn->property(PROPERTY_ID).toInt();
    setCursor(QCursor(btn->icon().pixmap(32, 32)));
#endif
}
//--------------------------------------------------------------------------------
void Labirint::load(void)
{

}
//--------------------------------------------------------------------------------
void Labirint::save(void)
{

}
//--------------------------------------------------------------------------------
void Labirint::updateText(void)
{

}
//--------------------------------------------------------------------------------
