/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#include "ui_test_opengl_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_opengl_mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init(); 
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_widgets();

    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    init_widgets();
    installEventFilter(this);

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test", &MainBox::test });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    //mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
void MainBox::init_widgets(void)
{
    ui->widget->setMinimumSize(800, 600);

    ui->dsb_X->setRange(-100, 100);
    ui->dsb_Y->setRange(-100, 100);
    ui->dsb_Z->setRange(-100, 100);

    ui->dsb_angle_X->setRange(-100, 100);
    ui->dsb_angle_Y->setRange(-100, 100);
    ui->dsb_angle_Z->setRange(-100, 100);

    ui->dsb_X->setSingleStep(0.01);
    ui->dsb_Y->setSingleStep(0.01);
    ui->dsb_Z->setSingleStep(0.01);

    ui->dsb_angle_X->setSingleStep(0.01);
    ui->dsb_angle_Y->setSingleStep(0.01);
    ui->dsb_angle_Z->setSingleStep(0.01);

    connect(ui->btn_get_X,  SIGNAL(clicked(bool)),  this,   SLOT(get_X()));
    connect(ui->btn_get_Y,  SIGNAL(clicked(bool)),  this,   SLOT(get_Y()));
    connect(ui->btn_get_Z,  SIGNAL(clicked(bool)),  this,   SLOT(get_Z()));

    connect(ui->btn_set_X,  SIGNAL(clicked(bool)),  this,   SLOT(set_X()));
    connect(ui->btn_set_Y,  SIGNAL(clicked(bool)),  this,   SLOT(set_Y()));
    connect(ui->btn_set_Z,  SIGNAL(clicked(bool)),  this,   SLOT(set_Z()));

    connect(ui->dsb_X,      SIGNAL(valueChanged(double)),   this,   SLOT(set_X()));
    connect(ui->dsb_Y,      SIGNAL(valueChanged(double)),   this,   SLOT(set_Y()));
    connect(ui->dsb_Z,      SIGNAL(valueChanged(double)),   this,   SLOT(set_Z()));

    connect(ui->btn_get_angle_X,    SIGNAL(clicked(bool)),  this,   SLOT(get_angle_X()));
    connect(ui->btn_get_angle_Y,    SIGNAL(clicked(bool)),  this,   SLOT(get_angle_Y()));
    connect(ui->btn_get_angle_Z,    SIGNAL(clicked(bool)),  this,   SLOT(get_angle_Z()));

    connect(ui->btn_set_angle_X,    SIGNAL(clicked(bool)),  this,   SLOT(set_angle_X()));
    connect(ui->btn_set_angle_Y,    SIGNAL(clicked(bool)),  this,   SLOT(set_angle_Y()));
    connect(ui->btn_set_angle_Z,    SIGNAL(clicked(bool)),  this,   SLOT(set_angle_Z()));

    connect(ui->dsb_angle_X,        SIGNAL(valueChanged(double)),   this,   SLOT(set_angle_X()));
    connect(ui->dsb_angle_Y,        SIGNAL(valueChanged(double)),   this,   SLOT(set_angle_Y()));
    connect(ui->dsb_angle_Z,        SIGNAL(valueChanged(double)),   this,   SLOT(set_angle_Z()));

    connect(ui->widget,     SIGNAL(info(QString)),  this,   SIGNAL(info(QString)));
    connect(ui->widget,     SIGNAL(debug(QString)), this,   SIGNAL(debug(QString)));
    connect(ui->widget,     SIGNAL(error(QString)), this,   SIGNAL(error(QString)));
    connect(ui->widget,     SIGNAL(trace(QString)), this,   SIGNAL(trace(QString)));

    connect(ui->cb_draw,        SIGNAL(toggled(bool)),  this,   SLOT(set_shown_object()));
    connect(ui->cb_sphere,      SIGNAL(toggled(bool)),  this,   SLOT(set_shown_object()));
    connect(ui->cb_cylinder,    SIGNAL(toggled(bool)),  this,   SLOT(set_shown_object()));
    connect(ui->cb_object,      SIGNAL(toggled(bool)),  this,   SLOT(set_shown_object()));
    connect(ui->cb_object2,     SIGNAL(toggled(bool)),  this,   SLOT(set_shown_object()));
    connect(ui->cb_cube,        SIGNAL(toggled(bool)),  this,   SLOT(set_shown_object()));
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok)
    {
        return;
    }
    auto cmd_it = std::find_if(
        commands.begin(),
        commands.end(),
        [cmd](CMD command){ return command.cmd == cmd; }
    );
    if (cmd_it != commands.end())
    {
        typedef bool (MainBox::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (this->*x)();
        }
        else
        {
            emit error("no func");
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::set_X(void)
{
    ui->widget->set_X(ui->dsb_X->value());
}
//--------------------------------------------------------------------------------
void MainBox::set_Y(void)
{
    ui->widget->set_Y(ui->dsb_Y->value());
}
//--------------------------------------------------------------------------------
void MainBox::set_Z(void)
{
    ui->widget->set_Z(ui->dsb_Z->value());
}
//--------------------------------------------------------------------------------
void MainBox::get_X(void)
{
    ui->dsb_X->setValue(ui->widget->get_x());
}
//--------------------------------------------------------------------------------
void MainBox::get_Y(void)
{
    ui->dsb_Y->setValue(ui->widget->get_y());
}
//--------------------------------------------------------------------------------
void MainBox::get_Z(void)
{
    ui->dsb_Z->setValue(ui->widget->get_z());
}
//--------------------------------------------------------------------------------
void MainBox::get_angle_X(void)
{
    ui->dsb_angle_X->setValue(ui->widget->get_angle_x());
}
//--------------------------------------------------------------------------------
void MainBox::get_angle_Y(void)
{
    ui->dsb_angle_Y->setValue(ui->widget->get_angle_y());
}
//--------------------------------------------------------------------------------
void MainBox::get_angle_Z(void)
{
    ui->dsb_angle_Z->setValue(ui->widget->get_angle_z());
}
//--------------------------------------------------------------------------------
void MainBox::set_angle_X(void)
{
    ui->widget->set_angle_X(ui->dsb_angle_X->value());
}
//--------------------------------------------------------------------------------
void MainBox::set_angle_Y(void)
{
    ui->widget->set_angle_Y(ui->dsb_angle_Y->value());
}
//--------------------------------------------------------------------------------
void MainBox::set_angle_Z(void)
{
    ui->widget->set_angle_Z(ui->dsb_angle_Z->value());
}
//--------------------------------------------------------------------------------
void MainBox::set_shown_object(void)
{
    ui->widget->show_object(ui->cb_draw->isChecked(),
                            ui->cb_sphere->isChecked(),
                            ui->cb_cylinder->isChecked(),
                            ui->cb_object->isChecked(),
                            ui->cb_object2->isChecked(),
                            ui->cb_cube->isChecked());
    ui->widget->updateGL();
}
//--------------------------------------------------------------------------------
bool MainBox::test(void)
{
    emit info("Test");

    ui->widget->draw_object();
    ui->widget->updateGL();

    return true;
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
