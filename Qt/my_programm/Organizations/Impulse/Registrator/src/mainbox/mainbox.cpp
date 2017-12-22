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
#include <QPushButton>
#include <QToolButton>
#include <QToolBar>
#include <QComboBox>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "multimeter_frame.hpp"
#include "v786_2_multimeter.hpp"
#include "grapherbox.hpp"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    QWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::connect_log(void)
{
    if(parentWidget())
    {
        connect(this, SIGNAL(info(QString)),    parentWidget(), SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)),   parentWidget(), SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)),   parentWidget(), SIGNAL(error(QString)));
        connect(this, SIGNAL(message(QString)), parentWidget(), SIGNAL(message(QString)));
    }
    else
    {
        connect(this, SIGNAL(info(QString)),    this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)),   this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)),   this, SLOT(log(QString)));
        connect(this, SIGNAL(message(QString)), this, SLOT(log(QString)));
    }
}
//--------------------------------------------------------------------------------
void MainBox::log(const QString &text)
{
    qDebug() << text;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);
    connect_log();

    createTestBar();

    frame_00 = new Multimeter_frame(0, this);
    frame_01 = new Multimeter_frame(1, this);
    frame_02 = new Multimeter_frame(2, this);
    frame_03 = new Multimeter_frame(3, this);
    frame_04 = new Multimeter_frame(4, this);
    frame_05 = new Multimeter_frame(5, this);
    frame_06 = new Multimeter_frame(6, this);
    frame_07 = new Multimeter_frame(7, this);
    frame_08 = new Multimeter_frame(8, this);
    frame_09 = new Multimeter_frame(9, this);
    frame_10 = new Multimeter_frame(10, this);
    frame_11 = new Multimeter_frame(11, this);
    frame_12 = new Multimeter_frame(12, this);
    frame_13 = new Multimeter_frame(13, this);
    frame_14 = new Multimeter_frame(14, this);
    frame_15 = new Multimeter_frame(15, this);

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->setSpacing(0);

    hbox->addWidget(frame_00);
    hbox->addWidget(frame_01);
    hbox->addWidget(frame_02);
    hbox->addWidget(frame_03);
    hbox->addWidget(frame_04);
    hbox->addWidget(frame_05);
    hbox->addWidget(frame_06);
    hbox->addWidget(frame_07);
    hbox->addWidget(frame_08);
    hbox->addWidget(frame_09);
    hbox->addWidget(frame_10);
    hbox->addWidget(frame_11);
    hbox->addWidget(frame_12);
    hbox->addWidget(frame_13);
    hbox->addWidget(frame_14);
    hbox->addWidget(frame_15);
    hbox->addStretch(1);

    multimeter = new Multimeter_V786_2(this);

    grapher = new GrapherBox(this);
    grapher->setMinimumHeight(600);
    curve_00 = grapher->add_curve("0");
    curve_01 = grapher->add_curve("1");
    curve_02 = grapher->add_curve("2");
    curve_03 = grapher->add_curve("3");
    curve_04 = grapher->add_curve("4");
    curve_05 = grapher->add_curve("5");
    curve_06 = grapher->add_curve("6");
    curve_07 = grapher->add_curve("7");
    curve_08 = grapher->add_curve("8");
    curve_09 = grapher->add_curve("9");
    curve_10 = grapher->add_curve("10");
    curve_11 = grapher->add_curve("11");
    curve_12 = grapher->add_curve("12");
    curve_13 = grapher->add_curve("13");
    curve_14 = grapher->add_curve("14");
    curve_15 = grapher->add_curve("15");

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(new QPushButton("test"));
    vbox->addLayout(hbox, 1);
    vbox->addWidget(grapher, 100);

    setLayout(vbox);
}
//--------------------------------------------------------------------------------
QToolButton *MainBox::add_button(QToolBar *tool_bar,
                                 QToolButton *tool_button,
                                 QIcon icon,
                                 const QString &text,
                                 const QString &tool_tip)
{
    if(!tool_bar) return NULL;
    if(!tool_button) return NULL;

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
    MainWindow *mw = (MainWindow *)parentWidget();
    if(!mw) return;

    QToolBar *toolBar = new QToolBar(tr("testbar"));
    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    QToolButton *btn_test = add_button(toolBar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");

    QToolButton *btn_find_device = add_button(toolBar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_BrowserReload),
                                              "find",
                                              "find");


    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
    connect(btn_find_device, SIGNAL(clicked()), this, SLOT(find_device()));
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info("test");
}
//--------------------------------------------------------------------------------
void MainBox::find_device(void)
{
    emit info("find");
}
//--------------------------------------------------------------------------------
void MainBox::block_interface(bool state)
{
    QList<QPushButton *> all_pushbutton = topLevelWidget()->findChildren<QPushButton *>();
    foreach(QPushButton *obj, all_pushbutton)
    {
        obj->setDisabled(state);
    }
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
