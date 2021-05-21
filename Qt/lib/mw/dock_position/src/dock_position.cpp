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
#include "mainwindow.hpp"
#include "dock_position.hpp"
#include "ui_dock_position.h"
//--------------------------------------------------------------------------------
Dock_position::Dock_position(const QString &doc_name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Dock_position)
{
    ui->setupUi(this);

    this->dock_name = doc_name;

#ifndef NORMAL_BUTTONS
    int size = 24;
    ui->btn_show->setFixedSize(size, size);
    ui->btn_up->setFixedSize(size, size);
    ui->btn_down->setFixedSize(size, size);
    ui->btn_left->setFixedSize(size, size);
    ui->btn_right->setFixedSize(size, size);

    ui->buttons_layout->setContentsMargins(0, 0, 0, 0);
    ui->buttons_layout->setSpacing(0);
#endif

    ui->btn_show->setToolTip(QObject::tr("Show buttons"));

    connect(ui->btn_show,   &QToolButton::toggled,  ui->btn_up,     &QToolButton::setVisible);
    connect(ui->btn_show,   &QToolButton::toggled,  ui->btn_down,   &QToolButton::setVisible);
    connect(ui->btn_show,   &QToolButton::toggled,  ui->btn_left,   &QToolButton::setVisible);
    connect(ui->btn_show,   &QToolButton::toggled,  ui->btn_right,  &QToolButton::setVisible);
    ui->btn_show->toggled(false);

    connect(ui->btn_up,     &QToolButton::clicked,  this,   &Dock_position::move_up);
    connect(ui->btn_down,   &QToolButton::clicked,  this,   &Dock_position::move_down);
    connect(ui->btn_left,   &QToolButton::clicked,  this,   &Dock_position::move_left);
    connect(ui->btn_right,  &QToolButton::clicked,  this,   &Dock_position::move_right);
}
//--------------------------------------------------------------------------------
Dock_position::~Dock_position()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Dock_position::move_up(void)
{
    MainWindow *mw = reinterpret_cast<MainWindow *>(topLevelWidget());
    if(mw)
    {
        QDockWidget *dw = mw->findChild<QDockWidget *>(dock_name);
        if(dw)
        {
            mw->addDockWidget(Qt::TopDockWidgetArea, dw);
        }
    }
    else
    {
#ifdef Q_DEBUG
        qDebug() << "mw is null!";
#endif
    }
}
//--------------------------------------------------------------------------------
void Dock_position::move_down(void)
{
    MainWindow *mw = reinterpret_cast<MainWindow *>(topLevelWidget());
    if(mw)
    {
        QDockWidget *dw = mw->findChild<QDockWidget *>(dock_name);
        if(dw)
        {
            mw->addDockWidget(Qt::BottomDockWidgetArea, dw);
        }
        else
        {
#ifdef Q_DEBUG
            qDebug() << dock_name << "not found!";
#endif
        }
    }
    else
    {
#ifdef Q_DEBUG
        qDebug() << "mw is null!";
#endif
    }
}
//--------------------------------------------------------------------------------
void Dock_position::move_left(void)
{
    MainWindow *mw = reinterpret_cast<MainWindow *>(topLevelWidget());
    if(mw)
    {
        QDockWidget *dw = mw->findChild<QDockWidget *>(dock_name);
        if(dw)
        {
            mw->addDockWidget(Qt::LeftDockWidgetArea, dw);
        }
        else
        {
#ifdef Q_DEBUG
            qDebug() << dock_name << "not found!";
#endif
        }
    }
    else
    {
#ifdef Q_DEBUG
        qDebug() << "mw is null!";
#endif
    }
}
//--------------------------------------------------------------------------------
void Dock_position::move_right(void)
{
    MainWindow *mw = reinterpret_cast<MainWindow *>(topLevelWidget());
    if(mw)
    {
        QDockWidget *dw = mw->findChild<QDockWidget *>(dock_name);
        if(dw)
        {
            mw->addDockWidget(Qt::RightDockWidgetArea, dw);
        }
        else
        {
#ifdef Q_DEBUG
            qDebug() << dock_name << "not found!";
#endif
        }
    }
    else
    {
#ifdef Q_DEBUG
        qDebug() << "mw is null!";
#endif
    }
}
//--------------------------------------------------------------------------------
