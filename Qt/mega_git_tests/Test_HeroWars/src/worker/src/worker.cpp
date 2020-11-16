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
    save_widgets();
    if(timer)
    {
        timer->disconnect();
        timer->deleteLater();
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void Worker::init(void)
{
    ui->setupUi(this);

    ui->btn_start->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_stop->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop));
    ui->btn_add->setIcon(QIcon(":/plus_minus/plus.png"));

    ui->sb_interval->setRange(0, 0xFFFF);

    connect(ui->btn_start,          &QPushButton::clicked,      this,   &Worker::f_start);
    connect(ui->btn_stop,           &QPushButton::clicked,      this,   &Worker::f_stop);
    connect(ui->btn_add,            &QPushButton::clicked,      this,   &Worker::f_add);

    connect(ui->btn_test,           &QPushButton::clicked,      this,   &Worker::f_test);
    connect(ui->btn_test_append,    &QPushButton::clicked,      this,   &Worker::f_test_append);
    connect(ui->sb_interval,        static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,                   &Worker::set_interval);

    ui->listWidget->setProperty(NO_SAVE, true);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout,    this,   &Worker::update);

    load_widgets();
}
//--------------------------------------------------------------------------------
int Worker::get_interval(void)
{
    return interval;
}
//--------------------------------------------------------------------------------
void Worker::set_interval(int value)
{
    interval = value;
}
//--------------------------------------------------------------------------------
void Worker::update(void)
{
    emit trace(Q_FUNC_INFO);
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

//    ui->listWidget->removeItemWidget(ui->listWidget->currentItem());
//    ui->listWidget->takeItem(1);
}
//--------------------------------------------------------------------------------
void Worker::f_test_append(void)
{
    QPixmap pixmap; //TODO он должен быть пустым для теста
    QListWidgetItem *lw = new QListWidgetItem(ui->listWidget);
    Item *item = new Item(ui->listWidget);
    item->set_pixmap(pixmap);
    lw->setSizeHint(item->sizeHint());
    ui->listWidget->setItemWidget(lw, item);

    item->setProperty(P_INDEX, ui->listWidget->currentIndex());

    connect(item,   &Item::remove_item, this,   &Worker::remove_item);
}
//--------------------------------------------------------------------------------
void Worker::f_start(void)
{
    if(timer)
    {
        timer->start(interval);
    }
}
//--------------------------------------------------------------------------------
void Worker::f_stop(void)
{
    if(timer)
    {
        timer->stop();
    }
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
