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
#include <QVBoxLayout>
#include <QMessageBox>
#include <QStringList>
#include <QTime>

#include <QAction>
#include <QMenu>

#include <QToolButton>
#include <QToolBar>
//--------------------------------------------------------------------------------
#include "ui_gy521_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "grapherbox.hpp"
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "gy521_mainbox.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox),
    serialBox(0)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

    serialBox = new SerialBox5(this, "RS232", "RS232");
    serialBox->add_menu(2);

    ui->serial_layout->addWidget(serialBox);
    ui->serial_layout->addStretch(1);

    grapher = new GrapherBox(this);
    curve_ACC_x = grapher->add_curve("ACC x");
    curve_ACC_y = grapher->add_curve("ACC y");
    curve_ACC_z = grapher->add_curve("ACC z");
    curve_TMP   = grapher->add_curve("TMP");
    curve_GYR_x = grapher->add_curve("GYR x");
    curve_GYR_y = grapher->add_curve("GYR y");
    curve_GYR_z = grapher->add_curve("GYR z");
    ui->grapher_layout->addWidget(grapher);

    connect(this, SIGNAL(send(QByteArray)), serialBox, SLOT(input(QByteArray)));
    connect(serialBox, SIGNAL(output(QByteArray)), this, SLOT(read_data(QByteArray)));
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");
    
    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info(tr("test"));
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray data)
{
    //emit trace(data);

    for(int n=0; n<data.length(); n++)
    {
        if(data[n] == '\r')
        {
            clean_data();
            data_rs232.clear();
        }
        else
        {
            data_rs232.append(data[n]);
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::clean_data(void)
{
    // AcX|AcY|AcZ|Tmp|GyX|GyY|GyZ

    //emit trace(data_rs232.data());

    QString str = data_rs232;
    QStringList sl = str.split('|');
    if(sl.count() != 7)
    {
        emit error(QString("sl.count() = %1").arg(sl.count()));
        data_rs232.clear();
        return;
    }
    int iAcX = sl.at(0).toInt();
    int iAcY = sl.at(1).toInt();
    int iAcZ = sl.at(2).toInt();
    int iTmp = sl.at(3).toInt();
    int iGyX = sl.at(4).toInt();
    int iGyY = sl.at(5).toInt();
    int iGyZ = sl.at(6).toInt();

    grapher->add_curve_data(curve_ACC_x,    iAcX);
    grapher->add_curve_data(curve_ACC_y,    iAcY);
    grapher->add_curve_data(curve_ACC_z,    iAcZ);

    grapher->add_curve_data(curve_TMP,      iTmp);

    grapher->add_curve_data(curve_GYR_x,    iGyX);
    grapher->add_curve_data(curve_GYR_y,    iGyY);
    grapher->add_curve_data(curve_GYR_z,    iGyZ);
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
