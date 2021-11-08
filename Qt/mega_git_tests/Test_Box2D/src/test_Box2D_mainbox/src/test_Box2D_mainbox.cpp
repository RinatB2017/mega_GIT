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
    save_widgets();
#ifdef QT_DEBUG
    qDebug() << "~MainBox()";
#endif
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

    set_ranges();
    set_single_steps();
    set_properties();
    set_connects();
    set_commands();
    set_icons();

    ui->world_widget->setFixedSize(WIDTH, HEIGHT);
    ui->world_widget->start();

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::set_ranges(void)
{
    ui->dsb_pt_to_pixel->setRange(0, 100000);

    ui->dsb_wall_x->setRange(0, 1000);
    ui->dsb_wall_y->setRange(0, 1000);
    ui->dsb_wall_w->setRange(0, 1000);
    ui->dsb_wall_h->setRange(0, 1000);
    ui->dsb_wall_angle->setRange(-359, 359);

    ui->dsb_ball_x->setRange(0, 1000);
    ui->dsb_ball_y->setRange(0, 1000);
    ui->dsb_ball_r->setRange(0, 1000);

    ui->dsb_bullet_x->setRange(0, 1000);
    ui->dsb_bullet_y->setRange(0, 1000);
    ui->dsb_bullet_r->setRange(0, 1);
    ui->dsb_bullet_linear_velocity_x->setRange(-1000, 1000);
    ui->dsb_bullet_linear_velocity_y->setRange(-1000, 1000);
    ui->dsb_bullet_impulse_x->setRange(-1000, 1000);
    ui->dsb_bullet_impulse_y->setRange(-1000, 1000);
    ui->dsb_bullet_point_x->setRange(-1000, 1000);
    ui->dsb_bullet_point_y->setRange(-1000, 1000);
}
//--------------------------------------------------------------------------------
void MainBox::set_single_steps(void)
{
#if 1
    QWidgetList l_widgets = qApp->allWidgets();
    for (int n=0; n<l_widgets.count(); n++)
    {
        QDoubleSpinBox *dsb = reinterpret_cast<QDoubleSpinBox *>(l_widgets.at(n));
        if(dsb)
        {
            if(dsb->objectName().left(4) == "dsb_")
            {
                dsb->setSingleStep(0.1);
            }
        }
    }
#else
    QList<QDoubleSpinBox *> l_dsb;

    l_dsb.append(ui->dsb_pixel_to_pt);
    l_dsb.append(ui->dsb_pt_to_pixel);

    l_dsb.append(ui->dsb_wall_x);
    l_dsb.append(ui->dsb_wall_y);
    l_dsb.append(ui->dsb_wall_w);
    l_dsb.append(ui->dsb_wall_h);
    l_dsb.append(ui->dsb_wall_angle);

    l_dsb.append(ui->dsb_ball_x);
    l_dsb.append(ui->dsb_ball_y);
    l_dsb.append(ui->dsb_ball_r);

    l_dsb.append(ui->dsb_bullet_x);
    l_dsb.append(ui->dsb_bullet_y);
    l_dsb.append(ui->dsb_bullet_r);
    l_dsb.append(ui->dsb_bullet_linear_velocity_x);
    l_dsb.append(ui->dsb_bullet_linear_velocity_y);
    l_dsb.append(ui->dsb_bullet_impulse_x);
    l_dsb.append(ui->dsb_bullet_impulse_y);
    l_dsb.append(ui->dsb_bullet_point_x);
    l_dsb.append(ui->dsb_bullet_point_y);

    foreach (QDoubleSpinBox *dsb, l_dsb)
    {
        dsb->setSingleStep(0.1);
    }
#endif
}
//--------------------------------------------------------------------------------
void MainBox::set_properties(void)
{
    ui->stackedWidget->setProperty(NO_SAVE, true);
    ui->btn_options->setProperty(NO_SAVE, true);
    ui->btn_ball->setProperty(NO_SAVE, true);
    ui->btn_wall->setProperty(NO_SAVE, true);
    ui->btn_bullet->setProperty(NO_SAVE, true);
}
//--------------------------------------------------------------------------------
void MainBox::set_connects(void)
{
    connect(ui->btn_options,    &QPushButton::clicked,  this,   &MainBox::page_select);
    connect(ui->btn_ball,       &QPushButton::clicked,  this,   &MainBox::page_select);
    connect(ui->btn_wall,       &QPushButton::clicked,  this,   &MainBox::page_select);
    connect(ui->btn_bullet,     &QPushButton::clicked,  this,   &MainBox::page_select);

    connect(ui->btn_choice_test,    &QToolButton::clicked,  this,               &MainBox::choice_test);
    connect(ui->btn_clear,          &QToolButton::clicked,  ui->world_widget,   &World::w_clear);

    connect(ui->btn_create_wall,    &QPushButton::clicked,  this,               &MainBox::test_create_wall);
    connect(ui->btn_create_ball,    &QPushButton::clicked,  this,               &MainBox::test_create_ball);
    connect(ui->btn_create_bullet,  &QPushButton::clicked,  this,               &MainBox::test_create_bullet);

    connect(ui->btn_pt_to_pixel_get,    &QToolButton::clicked,  this,   &MainBox::pt_to_pixel_get);
    connect(ui->btn_pt_to_pixel_set,    &QToolButton::clicked,  this,   &MainBox::pt_to_pixel_set);
}
//--------------------------------------------------------------------------------
void MainBox::set_commands(void)
{
    commands.clear();
    int id = 0;
    commands.append({ id++, "create_borders", &World::create_borders });
    commands.append({ id++, "create_scene_0", &World::create_scene_0 });
    commands.append({ id++, "create_scene_1", &World::create_scene_1 });
    commands.append({ id++, "create_scene_2", &World::create_scene_2 });
    commands.append({ id++, "create_scene_3", &World::create_scene_3 });
    commands.append({ id++, "create_scene_4", &World::create_scene_4 });
    commands.append({ id++, "create_scene_5", &World::create_scene_5 });

    ui->cb_tests->setProperty(NO_SAVE, true);
    foreach (CMD command, commands)
    {
        ui->cb_tests->addItem(command.cmd_text, QVariant(Qt::UserRole + command.cmd));
    }
}
//--------------------------------------------------------------------------------
void MainBox::set_icons(void)
{
    ui->btn_choice_test->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_clear->setIcon(qApp->style()->standardIcon(QStyle::SP_TrashIcon));
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw);

    QToolBar *testbar = new QToolBar("testbar");
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
    int cmd = ui->cb_tests->itemData(ui->cb_tests->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
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
void MainBox::page_select(void)
{
    QPushButton *btn = reinterpret_cast<QPushButton *>(sender());
    if(btn == ui->btn_options)
    {
        ui->stackedWidget->setCurrentWidget(ui->page_options);
    }
    if(btn == ui->btn_ball)
    {
        ui->stackedWidget->setCurrentWidget(ui->page_ball);
    }
    if(btn == ui->btn_wall)
    {
        ui->stackedWidget->setCurrentWidget(ui->page_wall);
    }
    if(btn == ui->btn_bullet)
    {
        ui->stackedWidget->setCurrentWidget(ui->page_bullet);
    }
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info("Test");
    ui->world_widget->test();
}
//--------------------------------------------------------------------------------
void MainBox::test_create_wall(void)
{
    Wall_param param;

    param.x = ui->dsb_wall_x->value();
    param.y = ui->dsb_wall_y->value();
    param.w = ui->dsb_wall_w->value();
    param.h = ui->dsb_wall_h->value();
    param.type = b2_staticBody;
    param.angle = ui->dsb_wall_angle->value();

    ui->world_widget->add_wall(param);
}
//--------------------------------------------------------------------------------
void MainBox::test_create_ball(void)
{
    Ball_param param;

    param.pos = b2Vec2(ui->dsb_ball_x->value(),
                       ui->dsb_ball_y->value());
    param.radius = ui->dsb_ball_r->value();
    param.type = b2_dynamicBody;

    ui->world_widget->add_ball(param);
}
//--------------------------------------------------------------------------------
void MainBox::test_create_bullet(void)
{
    Ball_param param;

    param.pos = b2Vec2(ui->dsb_bullet_x->value(),
                       ui->dsb_bullet_y->value());
    param.radius = ui->dsb_bullet_r->value();

    ui->world_widget->add_bullet(param,
                                 ui->dsb_bullet_linear_velocity_x->value(),
                                 ui->dsb_bullet_linear_velocity_y->value(),
                                 ui->dsb_bullet_impulse_x->value(),
                                 ui->dsb_bullet_impulse_y->value(),
                                 ui->dsb_bullet_point_x->value(),
                                 ui->dsb_bullet_point_y->value());
}
//--------------------------------------------------------------------------------
void MainBox::pt_to_pixel_get(void)
{
    ui->dsb_pt_to_pixel->setValue(ui->world_widget->pt_to_pixel_get());
}
//--------------------------------------------------------------------------------
void MainBox::pt_to_pixel_set(void)
{
    ui->world_widget->pt_to_pixel_set(ui->dsb_pt_to_pixel->value());
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
    int index = 0;
    bool ok = false;

    ok = load_int("index_tests", &index);
    if(ok)
    {
        ui->cb_tests->setCurrentIndex(index);
    }
}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{
    save_int("index_tests", ui->cb_tests->currentIndex());
}
//--------------------------------------------------------------------------------
