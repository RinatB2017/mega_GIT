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
#include "defines.hpp"
#include "worker.hpp"
#include "item.hpp"
#include "ui_worker.h"
//--------------------------------------------------------------------------------
Worker::Worker(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Worker)
{
    init();
}
//--------------------------------------------------------------------------------
Worker::~Worker()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Worker::init(void)
{
    ui->setupUi(this);

    ui->btn_start->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_stop->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop));
    ui->btn_add->setIcon(QIcon(":/plus_minus/plus.png"));

    connect(ui->btn_test,           &QPushButton::clicked,      this,   &Worker::f_test);
    connect(ui->btn_start,          &QPushButton::clicked,      this,   &Worker::f_start);
    connect(ui->btn_stop,           &QPushButton::clicked,      this,   &Worker::f_stop);
    connect(ui->btn_add,            &QPushButton::clicked,      this,   &Worker::f_add);

    ui->listWidget->setProperty(NO_SAVE, true);
}
//--------------------------------------------------------------------------------
void Worker::add_widget(const QPixmap &pixmap)
{
    this->pixmap = pixmap;
}
//--------------------------------------------------------------------------------
void Worker::f_test(void)
{
    emit info(QString("cnt %1").arg(ui->listWidget->count()));

    //ui->listWidget->removeItemWidget(ui->listWidget->currentItem());
    ui->listWidget->takeItem(1);
}
//--------------------------------------------------------------------------------
void Worker::f_start(void)
{
    fail();
}
//--------------------------------------------------------------------------------
void Worker::f_stop(void)
{
    fail();
}
//--------------------------------------------------------------------------------
void Worker::f_add(void)
{
    if(pixmap.isNull())
    {
        emit error("pixmap is empty");
        return;
    }

    QListWidgetItem *lw = new QListWidgetItem(ui->listWidget);
    Item *item = new Item(ui->listWidget);
    item->set_pixmap(pixmap);
    lw->setSizeHint(item->sizeHint());
    ui->listWidget->setItemWidget(lw, item);

    item->setProperty(P_INDEX, ui->listWidget->currentIndex());

    connect(item,   &Item::remove_item, this,   &Worker::remove_item);
}
//--------------------------------------------------------------------------------
void Worker::remove_item(int index)
{
    ui->listWidget->takeItem(index);
}
//--------------------------------------------------------------------------------
void Worker::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool Worker::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Worker::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void Worker::save_setting(void)
{

}
//--------------------------------------------------------------------------------
