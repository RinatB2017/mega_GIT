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
#include "autoclicker_widget.hpp"
#include "autoclicker_item.hpp"

#include "ui_autoclicker_widget.h"
//--------------------------------------------------------------------------------
AutoClicker_widget::AutoClicker_widget(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::AutoClicker_widget)
{
    ui->setupUi(this);

    init();
}
//--------------------------------------------------------------------------------
AutoClicker_widget::~AutoClicker_widget()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void AutoClicker_widget::init(void)
{
    ui->btn_clear->setIcon(QIcon(qApp->style()->standardIcon(QStyle::SP_TrashIcon)));

    connect(ui->btn_1,  &QToolButton::clicked,  this,   &AutoClicker_widget::click_1);
    connect(ui->btn_2,  &QToolButton::clicked,  this,   &AutoClicker_widget::click_2);
    connect(ui->btn_clear,  &QToolButton::clicked,  this,   &AutoClicker_widget::click_clear);
}
//--------------------------------------------------------------------------------
void AutoClicker_widget::click_1(void)
{
    AutoClicker_item *wgt = new AutoClicker_item();
    connect(wgt,    &AutoClicker_item::remove_item,  this,   &AutoClicker_widget::remove_item);
    wgt->set_name(QString("click_1: %1").arg(id));
    wgt->set_id(id);
    id++;

    QListWidgetItem* item = new QListWidgetItem(ui->lw_obj);
    item->setSizeHint(wgt->sizeHint());
    ui->lw_obj->setItemWidget(item, wgt);
}
//--------------------------------------------------------------------------------
void AutoClicker_widget::click_2(void)
{
    AutoClicker_item *wgt = new AutoClicker_item();
    connect(wgt,    &AutoClicker_item::remove_item,  this,   &AutoClicker_widget::remove_item);
    wgt->set_name(QString("click_2: %1").arg(id));
    wgt->set_id(id);
    id++;

    QListWidgetItem* item = new QListWidgetItem(ui->lw_obj);
    item->setSizeHint(wgt->sizeHint());
    ui->lw_obj->setItemWidget(item, wgt);
}
//--------------------------------------------------------------------------------
void AutoClicker_widget::click_clear(void)
{
    emit info("click_clear");

    id = 0;
    ui->lw_obj->clear();
}
//--------------------------------------------------------------------------------
void AutoClicker_widget::remove_item(void)
{
    AutoClicker_item *item = reinterpret_cast<AutoClicker_item *>(sender());
    if(item)
    {
        emit info(QString("remove_item: id = %1").arg(item->get_id()));
        for(int n=0; n<ui->lw_obj->count(); n++)
        {
            ui->lw_obj->setCurrentRow(n);
            QListWidgetItem *lw_item = ui->lw_obj->currentItem();
            if(lw_item)
            {
                AutoClicker_item *aw = static_cast<AutoClicker_item *>(ui->lw_obj->itemWidget(lw_item));
                if(aw->get_id() == item->get_id())
                {
                    ui->lw_obj->takeItem(n);
                    return;
                }
            }
        }
    }
}
//--------------------------------------------------------------------------------
void AutoClicker_widget::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool AutoClicker_widget::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void AutoClicker_widget::load_setting(void)
{
    emit debug("load_setting !");
}
//--------------------------------------------------------------------------------
void AutoClicker_widget::save_setting(void)
{
    emit debug("save_setting !");
}
//--------------------------------------------------------------------------------
