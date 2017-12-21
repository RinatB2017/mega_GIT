/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#include <QDebug>
//--------------------------------------------------------------------------------
#include "mymainwindow.hpp"
//--------------------------------------------------------------------------------
MyMainWindow::MyMainWindow(MainWindow *parent) :
    MainWindow(parent)
{
    qDebug() << "MyMainWindow()";

    create_local_menus();
}
//--------------------------------------------------------------------------------
MyMainWindow::~MyMainWindow()
{
    qDebug() << "~NyMainWindow()";
}
//--------------------------------------------------------------------------------
#ifndef NO_MENU
void MyMainWindow::create_local_menus(void)
{
    save_action = new QAction(this);
    save_action->setText(QObject::tr("save"));
    save_action->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogSaveButton));

    connect(save_action,    SIGNAL(triggered()),
            this,           SLOT(save_setting()));

    get_file_menu()->addSeparator();
    get_file_menu()->addAction(save_action);
}
#endif
//--------------------------------------------------------------------------------
