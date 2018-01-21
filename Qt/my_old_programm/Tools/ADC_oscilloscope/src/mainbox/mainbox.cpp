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
#include <QCoreApplication>
#include <QVBoxLayout>
#include <QToolButton>
#include <QCheckBox>
#include <QComboBox>
#include <QToolBar>
#include <QThread>
#include <QLabel>
#include <QDebug>
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
#include <QSerialPort>
//--------------------------------------------------------------------------------
#include "modbusasciiwidget.hpp"
#include "oscilloscopebox.hpp"
#include "mainwindow.hpp"
#include "ui_mainbox.h"
#include "mainbox.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox),
    parent(parent)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init()
{
    //y[n] = double(308.0)*qSin(double(n)*double(M_PI)/double(180.0));;
    ui->setupUi(this);

    createSerialBar();
    createTestBar();

    oscilloscope = new OscilloscopeBox(this);
    //connect(widget, SIGNAL(get_data(DATA)), this, SLOT(get_data(DATA)));
    connect(widget, SIGNAL(get_data(QByteArray)), this, SLOT(get_data(QByteArray)));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(oscilloscope);
    setLayout(vbox);

    updateText();

    setMinimumWidth(1024);
}
//--------------------------------------------------------------------------------
void MainBox::get_data(const QByteArray &data)
{
    //emit info(QString("get %1 bytes").arg(data.count()));
    for(int n=0; n<data.count(); n+=8)
    {
        unsigned short reg_0 = ((data.at(n) << 8)   & 0xFF00) | (data.at(n+1) & 0x00FF);
        unsigned short reg_1 = ((data.at(n+2) << 8) & 0xFF00) | (data.at(n+3) & 0x00FF);
        unsigned short reg_2 = ((data.at(n+4) << 8) & 0xFF00) | (data.at(n+5) & 0x00FF);
        unsigned short reg_3 = ((data.at(n+6) << 8) & 0xFF00) | (data.at(n+7) & 0x00FF);

        //qDebug() << reg_0 << reg_1 << reg_2 << reg_3;

        oscilloscope->add_curve_data(0, reg_0);
        oscilloscope->add_curve_data(1, reg_1);
        oscilloscope->add_curve_data(2, reg_2);
        oscilloscope->add_curve_data(3, reg_3);
    }
}
//--------------------------------------------------------------------------------
void MainBox::get_data(const DATA &data)
{
#if 0
    emit debug(QString("%1 %2 %3 %4")
               .arg(data.data.reg_0)
               .arg(data.data.reg_1)
               .arg(data.data.reg_2)
               .arg(data.data.reg_3));
#endif

    //add_data_2byte(0, data.data.reg_0);
    //add_data_2byte(1, data.data.reg_1);
    //add_data_2byte(2, data.data.reg_2);
    //add_data_2byte(3, data.data.reg_3);

    oscilloscope->add_curve_data(0, data.data.reg_0);
    oscilloscope->add_curve_data(1, data.data.reg_1);
    oscilloscope->add_curve_data(2, data.data.reg_2);
    oscilloscope->add_curve_data(3, data.data.reg_3);
}
//--------------------------------------------------------------------------------
void MainBox::add_data_1byte(unsigned int index_curve, int8_t data)
{
    oscilloscope->add_curve_data(index_curve, data);
}
//--------------------------------------------------------------------------------
void MainBox::add_data_2byte(unsigned int index_curve, int16_t data)
{
    oscilloscope->add_curve_data(index_curve, data);
}
//--------------------------------------------------------------------------------
void MainBox::add_data_4byte(unsigned int index_curve, int32_t data)
{
    oscilloscope->add_curve_data(index_curve, data);
}
//--------------------------------------------------------------------------------
void MainBox::add_data_float(unsigned int index_curve, float data)
{
    oscilloscope->add_curve_data(index_curve, data);
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
                                       qApp->style()->standardIcon(QStyle::SP_CommandLink),
                                       "test",
                                       "test");
    
    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::createSerialBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    widget = new ModbusAsciiWidget(this);

    QToolBar *testbar = new QToolBar("testbar");
    Q_CHECK_PTR(testbar);

    testbar->addWidget(widget);
    mw->addToolBar(Qt::TopToolBarArea, testbar);
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit info("test");
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
