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
#include <QTreeWidget>
#include <QHeaderView>
//--------------------------------------------------------------------------------
#include "menudock.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MenuDock::MenuDock(QWidget *parent) :
    QDockWidget(parent)
{
    setWindowTitle("MenuDock");

    tree = new QTreeWidget(this);
    tree->header()->hide();

    QTreeWidgetItem *i_main_options = new QTreeWidgetItem(tree);
    i_main_options->setText(0, "NAME_MAIN_OPTIONS");

    setWidget(tree);
    connect(tree, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(itemClicked(QTreeWidgetItem*,int)));
}
//--------------------------------------------------------------------------------
MenuDock::~MenuDock()
{
    if(tree)
    {
        delete tree;
    }
}
//--------------------------------------------------------------------------------
void MenuDock::itemClicked(QTreeWidgetItem *item, int index)
{
    emit info(QString("item %1").arg(item->text(0)));
    emit info(QString("index %1").arg(index));
}
//--------------------------------------------------------------------------------
