/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#include "mylistwidget.hpp"
//--------------------------------------------------------------------------------
MyListWidget::MyListWidget(QWidget *parent) :
    QListWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
MyListWidget::~MyListWidget()
{

}
//--------------------------------------------------------------------------------
void MyListWidget::init(void)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,   &QListWidget::customContextMenuRequested,   this,   &MyListWidget::popup);
}
//--------------------------------------------------------------------------------
void MyListWidget::delete_item(void)
{
    QList<QListWidgetItem*> items = selectedItems();
    foreach(QListWidgetItem* item, items)
    {
        removeItemWidget(item);
        delete item;
    }
}
//--------------------------------------------------------------------------------
void MyListWidget::popup(QPoint)
{
    QMenu *popup_menu = new QMenu(this);

    QAction *delete_action = new QAction(qApp->style()->standardIcon(QStyle::SP_TrashIcon),
                                         QObject::tr("delete item"),
                                         this);

    connect(delete_action,  &QAction::triggered,    [this]() {
        QList<QListWidgetItem*> items = selectedItems();

        foreach(QListWidgetItem* item, items)
        {
            removeItemWidget(item);
            delete item;
        }
    });

    popup_menu->addAction(delete_action);
    popup_menu->exec(QCursor::pos());
}
//--------------------------------------------------------------------------------
void MyListWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete)
    {
        QList<QListWidgetItem*> items = selectedItems();

        foreach(QListWidgetItem* item, items)
        {
            removeItemWidget(item);
            delete item;
        }
    }
}
//--------------------------------------------------------------------------------
