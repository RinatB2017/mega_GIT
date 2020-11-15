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
#include "item_options.hpp"
#include "item.hpp"
#include "ui_item.h"
//--------------------------------------------------------------------------------
Item::Item(QListWidget *lw, QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Item)
{
    parent_lw = lw;
    init();
}
//--------------------------------------------------------------------------------
Item::~Item()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Item::init(void)
{
    ui->setupUi(this);

    ui->btn_clear->setIcon(qApp->style()->standardIcon(QStyle::SP_TrashIcon));
    ui->btn_click->setIcon(qApp->style()->standardIcon(QStyle::SP_CommandLink));
    ui->btn_load->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogOpenButton));
    ui->btn_save->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogSaveButton));

    connect(ui->btn_clear,  &QToolButton::clicked,  this,   &Item::item_close);
    connect(ui->btn_click,  &QToolButton::clicked,  this,   &Item::item_click);
    connect(ui->btn_load,   &QToolButton::clicked,  this,   &Item::item_load);
    connect(ui->btn_save,   &QToolButton::clicked,  this,   &Item::item_save);
}
//--------------------------------------------------------------------------------
void Item::item_close(void)
{
    fail();
//    parent_lw->removeItemWidget(parent_lw->itemWidget(static_cast<QListWidgetItem *>(property("index").toModelIndex())));
}
//--------------------------------------------------------------------------------
void Item::item_click(void)
{
    fail();
}
//--------------------------------------------------------------------------------
void Item::item_load(void)
{
    fail();
}
//--------------------------------------------------------------------------------
void Item::item_save(void)
{
    fail();
}
//--------------------------------------------------------------------------------
void Item::set_pixmap(const QPixmap &pixmap)
{
    ui->lbl_picture->setPixmap(pixmap);
}
//--------------------------------------------------------------------------------
void Item::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool Item::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Item::load_setting(void)
{
//    emit info(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
void Item::save_setting(void)
{
//    emit info(Q_FUNC_INFO);
}
//--------------------------------------------------------------------------------
