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
//--------------------------------------------------------------------------------
#include <QToolButton>
#include <QToolBar>
#include <QWidget>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "serialbox.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
//--------------------------------------------------------------------------------
#include "abstractserial.h"
//--------------------------------------------------------------------------------
#include "worker.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox),
    serial232(0),
    worker(0)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete serial232;
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();
    create_widgets();
    create_navigator();
}
//--------------------------------------------------------------------------------
void MainBox::create_widgets(void)
{
    serial232 = new SerialBox(this, tr("RS232"));

    ui->layout_USB0->addWidget(serial232);
}
//--------------------------------------------------------------------------------
void MainBox::create_navigator(void)
{
    worker = new Worker(this, serial232);
    connect(worker, SIGNAL(output_latitude_string(QString)),    ui->le_Latitude,    SLOT(setText(QString)));
    connect(worker, SIGNAL(output_longitude_string(QString)),   ui->le_Longitude,   SLOT(setText(QString)));
    connect(worker, SIGNAL(output_observation(QString)),        ui->le_Time,        SLOT(setText(QString)));
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    QToolButton *btnTest = new QToolButton(this);
    btnTest->setText("test");
    btnTest->setIcon(QIcon::fromTheme(QLatin1String("applications-system")));

    QToolBar *toolBar = new QToolBar(this);

    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(mw)
    {
        mw->addToolBar(Qt::TopToolBarArea, toolBar);
    }

    toolBar->addWidget(btnTest);

    connect(btnTest, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info("Test");
}
//--------------------------------------------------------------------------------
void MainBox::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
SerialBox* MainBox::get_serial(void)
{
    return serial232;
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
