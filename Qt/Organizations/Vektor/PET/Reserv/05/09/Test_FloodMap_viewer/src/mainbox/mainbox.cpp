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
#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QToolButton>
#include <QSplitter>
#include <QCheckBox>
#include <QComboBox>
#include <QToolBar>
#include <QDebug>
#include <QtMath>
//--------------------------------------------------------------------------------
#include <qwt_color_map.h>
#include <qwt_scale_widget.h>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
    load_config();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_config();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();
    create_color_map();

    //---
    ui->Thermo->setColorMap(colorMap);
    //ui->Thermo->setScale(0, 105);
    ui->Thermo->setValue(100);
    ui->Thermo->update();
    //---
    ui->floodmap_widget->draw(QImage(":/floodmap/color/test_floodmap_512.png"), 4*0xFFFF, true);
    //---

    if(sizeHint().height() > 0)
    {
        setMinimumHeight(sizeHint().height());
    }
}
//--------------------------------------------------------------------------------
QToolButton *MainBox::add_button(QToolBar *tool_bar,
                                 QToolButton *tool_button,
                                 QIcon icon,
                                 const QString &text,
                                 const QString &tool_tip)
{
    Q_CHECK_PTR(tool_bar);
    Q_CHECK_PTR(tool_button);
    if(tool_bar == nullptr)
    {
        return nullptr;
    }
    if(tool_button == nullptr)
    {
        return nullptr;
    }

    tool_button->setIcon(icon);
    tool_button->setText(text);
    tool_button->setToolTip(tool_tip);
    tool_button->setObjectName(text);
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(mw == nullptr)
    {
        return;
    }

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &MainBox::test_0 });
    commands.append({ ID_TEST_1, "test 1", &MainBox::test_1 });
    commands.append({ ID_TEST_2, "test 2", &MainBox::test_2 });
    commands.append({ ID_TEST_3, "test 3", &MainBox::test_3 });
    commands.append({ ID_TEST_4, "test 4", &MainBox::test_4 });
    commands.append({ ID_TEST_5, "test 5", &MainBox::test_5 });
    commands.append({ ID_TEST_6, "test 6", 0 });

    QToolBar *toolBar = new QToolBar(tr("testbar"));
    toolBar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    QCheckBox *cb_block = new QCheckBox("block");
    toolBar->addWidget(cb_block);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
    }

    toolBar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(toolBar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    connect(cb_block, SIGNAL(clicked(bool)), cb_test,           SLOT(setDisabled(bool)));
    connect(cb_block, SIGNAL(clicked(bool)), btn_choice_test,   SLOT(setDisabled(bool)));

    //toolBar->setFixedWidth(toolBar->sizeHint().width());
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok) return;
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef bool (MainBox::*function)(void);
            function x;
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
bool MainBox::test_0(void)
{
    emit info("Test_0()");

    ui->floodmap_widget->draw(QImage(":/floodmap/color/test_floodmap_512.png"), 4*0xFFFF, true);
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_1()");

    ui->floodmap_widget->draw(QImage(":/floodmap/mono/test_floodmap_512.png"), 4*0xFFFF, false);
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
void MainBox::create_color_map(void)
{
    colorMap = new QwtLinearColorMap();

    colorMap->setColorInterval(QColor(0,    0,      0),
                               QColor(255,  255,    255));

    double k = 0.5;

    colorMap->addColorStop(0.0,     QColor(0,   0,   0));
    colorMap->addColorStop(0.2 * k, QColor(85,  0,   170));
    colorMap->addColorStop(0.4 * k, QColor(170, 0,   170));
    colorMap->addColorStop(0.6 * k, QColor(255, 85,  0));
    colorMap->addColorStop(0.8 * k, QColor(255, 170, 0));
    colorMap->addColorStop(1.0,     QColor(255, 255, 255));
}
//--------------------------------------------------------------------------------
bool MainBox::eventFilter(QObject *, QEvent *e)
{
    if (e->type() == QEvent::MouseButtonRelease)
    {
        if (static_cast<QMouseEvent*>(e)->button() == Qt::RightButton)
        {
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
void MainBox::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
