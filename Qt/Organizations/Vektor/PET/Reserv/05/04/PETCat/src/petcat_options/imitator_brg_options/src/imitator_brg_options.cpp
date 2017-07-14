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
#include "imitator_brg_options.hpp"
#include "ui_imitator_brg_options.h"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "petcat_options.hpp"
//--------------------------------------------------------------------------------
Imitator_BRG_options::Imitator_BRG_options(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Imitator_BRG_options)
{
    init();
}
//--------------------------------------------------------------------------------
Imitator_BRG_options::~Imitator_BRG_options()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Imitator_BRG_options::init(void)
{
    ui->setupUi(this);

    block_wheel();

    ui->sb_vid->setRange(0, 0xFFFF);
    ui->sb_pid->setRange(0, 0xFFFF);

    QVariant imitator_brg_vid = IMITATOR_BRG_DEFAULT_VID;
    QVariant imitator_brg_pid = IMITATOR_BRG_DEFAULT_PID;

    PETCat_options *o = new PETCat_options();
    o->get_imitator_brg_option(PARAM_imitator_brg_vid,    &imitator_brg_vid);
    o->get_imitator_brg_option(PARAM_imitator_brg_pid,    &imitator_brg_pid);
    o->deleteLater();

    ui->sb_vid->setValue(imitator_brg_vid.toInt());
    ui->sb_pid->setValue(imitator_brg_pid.toInt());

    connect(ui->btn_accept, SIGNAL(clicked(bool)),  this,   SLOT(t_accept()));
}
//--------------------------------------------------------------------------------
void Imitator_BRG_options::block_wheel(void)
{
    QList<QWidget *> all_obj = findChildren<QWidget *>();
    foreach(QWidget *obj, all_obj)
    {
        if(!obj->objectName().isEmpty())
        {
            obj->installEventFilter(this);
        }
    }
}
//--------------------------------------------------------------------------------
void Imitator_BRG_options::t_accept(void)
{
    PETCat_options *o = new PETCat_options();
    o->set_imitator_brg_option(PARAM_imitator_brg_pid,  ui->sb_pid->value());
    o->set_imitator_brg_option(PARAM_imitator_brg_vid,  ui->sb_vid->value());
    o->deleteLater();

    messagebox_info("Сохранение", "OK");
}
//--------------------------------------------------------------------------------
