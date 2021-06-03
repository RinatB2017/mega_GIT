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
#include "ui_mainwidget_gui.h"
//--------------------------------------------------------------------------------
#include "mainwidget_gui.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainWidget_GUI::MainWidget_GUI(QWidget *parent,
                               MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainWidget_GUI)
{
    init();
}
//--------------------------------------------------------------------------------
MainWidget_GUI::~MainWidget_GUI()
{
    save_widgets();
#ifdef QT_DEBUG
    qDebug() << "~MainWidget_GUI()";
#endif
    delete ui;
}
//--------------------------------------------------------------------------------
void MainWidget_GUI::init(void)
{
    ui->setupUi(this);
    load_widgets();
}
//--------------------------------------------------------------------------------
void MainWidget_GUI::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainWidget_GUI::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainWidget_GUI::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainWidget_GUI::save_setting(void)
{

}
//--------------------------------------------------------------------------------
