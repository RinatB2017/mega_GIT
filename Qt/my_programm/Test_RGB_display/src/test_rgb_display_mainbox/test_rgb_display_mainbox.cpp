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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include <QImage>
//--------------------------------------------------------------------------------
#include "ui_test_rgb_display_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_rgb_display_mainbox.hpp"
#include "rgb_display_led.hpp"
#include "rgb_display.hpp"
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
    //save_leds();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

    connect(this,   SIGNAL(send(QByteArray)),   ui->serial_widget,  SLOT(input(QByteArray)));
    connect(ui->serial_widget,  SIGNAL(output(QByteArray)), this,   SLOT(read_data(QByteArray)));

    connect(ui->btn_ul, SIGNAL(clicked(bool)),  this,   SLOT(move_ul()));
    connect(ui->btn_u,  SIGNAL(clicked(bool)),  this,   SLOT(move_u()));
    connect(ui->btn_ur, SIGNAL(clicked(bool)),  this,   SLOT(move_ur()));

    connect(ui->btn_l,  SIGNAL(clicked(bool)),  this,   SLOT(move_l()));
    connect(ui->btn_r,  SIGNAL(clicked(bool)),  this,   SLOT(move_r()));

    connect(ui->btn_dl, SIGNAL(clicked(bool)),  this,   SLOT(move_dl()));
    connect(ui->btn_d,  SIGNAL(clicked(bool)),  this,   SLOT(move_d()));
    connect(ui->btn_dr, SIGNAL(clicked(bool)),  this,   SLOT(move_dr()));

    connect(ui->btn_load_ico,   SIGNAL(clicked(bool)),  ui->rgb_display,   SLOT(load_ico()));
    connect(ui->btn_load_pic,   SIGNAL(clicked(bool)),  ui->rgb_display,   SLOT(load_pic()));

    //setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &MainBox::test_0 });
    commands.append({ ID_TEST_1, "test 1", &MainBox::test_1 });
    commands.append({ ID_TEST_2, "test 2", &MainBox::test_2 });
    commands.append({ ID_TEST_3, "test 3", &MainBox::test_3 });
    commands.append({ ID_TEST_4, "test 4", &MainBox::test_4 });
    commands.append({ ID_TEST_5, "test 5", &MainBox::test_5 });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_block = new QCheckBox("block", this);
    testbar->addWidget(cb_block);

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

    connect(cb_block, SIGNAL(clicked(bool)), cb_test,           SLOT(setDisabled(bool)));
    connect(cb_block, SIGNAL(clicked(bool)), btn_choice_test,   SLOT(setDisabled(bool)));

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
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
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef bool (MainBox::*my_mega_function)(void);
            my_mega_function x;
            x = command.func;
            if(x)
            {
                (this->*x)();
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
bool MainBox::prepare_data(QByteArray input, QByteArray *output)
{
    Q_CHECK_PTR(output);

    if(input.isEmpty())
    {
        emit error("Input is empty!");
        return false;
    }
    output->clear();

    output->append(':');
    output->append(input.toHex().toUpper());
    output->append('\n');

    return true;
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray data)
{
    emit trace("read_data");
    emit debug(data.data());
}
//--------------------------------------------------------------------------------
void MainBox::move_ul(void)
{
    emit trace(Q_FUNC_INFO);

    if(begin_x > 0)
    {
        begin_x--;
    }
    if(begin_y > 0)
    {
        begin_y--;
    }
    ui->rgb_display->show_picture(begin_x, begin_y);
}
//--------------------------------------------------------------------------------
void MainBox::move_u(void)
{
    emit trace(Q_FUNC_INFO);

    if(begin_y > 0)
    {
        begin_y--;
    }
    ui->rgb_display->show_picture(begin_x, begin_y);
}
//--------------------------------------------------------------------------------
void MainBox::move_ur(void)
{
    emit trace(Q_FUNC_INFO);

    if((begin_x < ui->rgb_display->get_max_x())  && ((begin_x + ui->rgb_display->get_max_x()) < ui->rgb_display->get_picture_w()))
    {
        begin_x++;
    }
    if((begin_y < ui->rgb_display->get_max_y())  && ((begin_y + ui->rgb_display->get_max_y()) < ui->rgb_display->get_picture_h()))
    {
        begin_y++;
    }
    ui->rgb_display->show_picture(begin_x, begin_y);
}
//--------------------------------------------------------------------------------
void MainBox::move_l(void)
{
    emit trace(Q_FUNC_INFO);

    if(begin_x > 0)
    {
        begin_x--;
    }
    ui->rgb_display->show_picture(begin_x, begin_y);
}
//--------------------------------------------------------------------------------
void MainBox::move_r(void)
{
    emit trace(Q_FUNC_INFO);

    if((begin_x < ui->rgb_display->get_max_x()) && ((begin_x + ui->rgb_display->get_max_x()) < ui->rgb_display->get_picture_w()))
    {
        begin_x++;
    }
    ui->rgb_display->show_picture(begin_x, begin_y);
}
//--------------------------------------------------------------------------------
void MainBox::move_dl(void)
{
    emit trace(Q_FUNC_INFO);

    if(begin_x > 0)
    {
        begin_x--;
    }
    if((begin_y < ui->rgb_display->get_max_y())  && ((begin_y + ui->rgb_display->get_max_y()) < ui->rgb_display->get_picture_h()))
    {
        begin_y++;
    }
    ui->rgb_display->show_picture(begin_x, begin_y);
}
//--------------------------------------------------------------------------------
void MainBox::move_d(void)
{
    emit trace(Q_FUNC_INFO);

    if((begin_y < ui->rgb_display->get_max_y())  && ((begin_y + ui->rgb_display->get_max_y()) < ui->rgb_display->get_picture_h()))
    {
        begin_y++;
    }
    ui->rgb_display->show_picture(begin_x, begin_y);
}
//--------------------------------------------------------------------------------
void MainBox:: move_dr(void)
{
    emit trace(Q_FUNC_INFO);

    if((begin_x < ui->rgb_display->get_max_x())  && ((begin_x + ui->rgb_display->get_max_x()) < ui->rgb_display->get_picture_w()))
    {
        begin_x++;
    }
    if((begin_y < ui->rgb_display->get_max_y())  && ((begin_y + ui->rgb_display->get_max_y()) < ui->rgb_display->get_picture_h()))
    {
        begin_y++;
    }
    ui->rgb_display->show_picture(begin_x, begin_y);
}
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit info("Test_0()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_1()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit info("Test_2()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit info("Test_3()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit info("Test_4()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit info("Test_5()");

    return true;
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
