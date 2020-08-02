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
#include "dragdrop_widget.hpp"
//--------------------------------------------------------------------------------
DragDrop_widget::DragDrop_widget(QWidget *parent) :
    QLabel(parent)
{
    setAcceptDrops(true);
}
//--------------------------------------------------------------------------------
DragDrop_widget::~DragDrop_widget()
{

}
//--------------------------------------------------------------------------------
void DragDrop_widget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        event->accept();
    }
}
//--------------------------------------------------------------------------------
void DragDrop_widget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        QList<QUrl> urls = event->mimeData()->urls();
        QString filePath = urls.at(0).toLocalFile();
        if(filePath.isEmpty() == false)
        {
            setPixmap(QPixmap(filePath));
        }
        event->accept();
    }
}
//--------------------------------------------------------------------------------
