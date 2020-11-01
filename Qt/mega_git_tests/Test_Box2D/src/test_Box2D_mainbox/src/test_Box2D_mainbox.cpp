/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#include <QLCDNumber>
//--------------------------------------------------------------------------------
#include "ui_test_Box2D_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "test_Box2D_mainbox.hpp"
#include "defines.hpp"
#include "world.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    qDebug() << "~MainBox()";
    delete ui;

    qApp->quit();
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    ui->lcd->setSegmentStyle(QLCDNumber::Flat);
    ui->lcd->setDigitCount(10);
    ui->lcd->setStyleSheet("background-color:black; color:green;");
    ui->lcd->setMinimumSize(16*10, 32);

    commands.clear();
    int id = 0;
    commands.append({ id++, "create_scene_0", &World::create_scene_0 });
    commands.append({ id++, "create_scene_1", &World::create_scene_1 });
    commands.append({ id++, "create_scene_2", &World::create_scene_2 });
    commands.append({ id++, "create_scene_3", &World::create_scene_3 });

    foreach (CMD command, commands)
    {
        ui->cb_block_insert_object->addItem(command.cmd_text, QVariant(Qt::UserRole + command.cmd));
    }

    ui->btn_choice_test->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_clear->setIcon(qApp->style()->standardIcon(QStyle::SP_TrashIcon));

    connect(ui->world_widget,   &World::cnt_objects,
            ui->lcd,            static_cast<void (QLCDNumber::*)(int)>(&QLCDNumber::display));

    ui->world_widget->setFixedSize(WIDTH, HEIGHT);

    connect(ui->btn_choice_test,    &QToolButton::clicked,  this,               &MainBox::choice_test);
    connect(ui->btn_clear,          &QToolButton::clicked,  ui->world_widget,   &World::w_clear); //FIXME непонятно, почему крашится. Надо разобраться

    ui->world_widget->start();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar(tr("testbar"));
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test,    &QToolButton::clicked,  this,   &MainBox::test);
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = ui->cb_block_insert_object->itemData(ui->cb_block_insert_object->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;
    auto cmd_it = std::find_if(
                commands.begin(),
                commands.end(),
                [cmd](CMD command){ return command.cmd == cmd; }
            );
    if (cmd_it != commands.end())
    {
        typedef void (World::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (ui->world_widget->*x)();
        }
        else
        {
            emit error("no func");
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info("Test");
#if 1
    ui->world_widget->create_scene_0();
    ui->world_widget->create_scene_1();
    ui->world_widget->create_scene_2();
    ui->world_widget->create_scene_3();
#endif
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
