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
#include "ui_Test_Box2D_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "Test_Box2D_mainbox.hpp"
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
    world->deleteLater();
    qApp->quit();

    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    world = new World(this);

    createTestBar();

    connect(world, SIGNAL(cnt_objects(int)), lcd, SLOT(display(int)));
    connect(cb_block_insert_object, SIGNAL(clicked(bool)), world, SLOT(block_insert_objects(bool)));

    world->setFixedSize(WIDTH, HEIGHT);
    world->show();

    //world->create_scene_0();
    //world->create_scene_1();
    //world->create_scene_2();
    //world->create_scene_3();

    world->start();

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setMargin(0);
    vbox->setSpacing(0);
    vbox->addWidget(world);
    setLayout(vbox);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar(tr("testbar"));
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    commands.clear();
    commands.append({ 0x10, "create_scene_0", &World::create_scene_0 });
    commands.append({ 0x20, "create_scene_1", &World::create_scene_1 });
    commands.append({ 0x39, "create_scene_2", &World::create_scene_2 });
    commands.append({ 0x40, "create_scene_3", &World::create_scene_3 });

#if 1
    lcd = new QLCDNumber(this);
    lcd->setSegmentStyle(QLCDNumber::Flat);
    lcd->setDigitCount(10);
    lcd->setStyleSheet("background-color:black; color:green;");
    lcd->setMinimumSize(16*10, 32);
    testbar->addWidget(lcd);
#endif

#if 0
    cb_block_insert_object = new QCheckBox("block insert objects");
    cb_block_insert_object->setObjectName("cb_block_insert_object");
    testbar->addWidget(cb_block_insert_object);
#endif

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(Qt::UserRole + command.cmd));
    }

    testbar->addWidget(cb_test);

    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    QToolButton *btn_clear = add_button(testbar,
                                     new QToolButton(this),
                                     qApp->style()->standardIcon(QStyle::SP_MessageBoxWarning),
                                     "clear",
                                     "clear");
    btn_clear->setObjectName("btn_clear");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));
    connect(btn_clear, SIGNAL(clicked(bool)), world, SLOT(clear()));
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef void (World::*function)(void);
            function x;
            x = command.func;
            if(x)
            {
                (world->*x)();
            }
            else
            {
                emit error("no func");
            }

            return;
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    //world->test();

    //world->stop();
    //world->delete_objects();
    //world->create_scene_3();
    //world->start();

    world->deleteLater();
    qApp->quit();
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
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
