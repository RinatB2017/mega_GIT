/*********************************************************************************
**                                                                              **
**     Copyright (C) 2021                                                       **
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
#include "table_widget.hpp"
//--------------------------------------------------------------------------------
Table_widget::Table_widget(QWidget *parent) :
    QTableWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
Table_widget::~Table_widget()
{

}
//--------------------------------------------------------------------------------
void Table_widget::init(void)
{
    setColumnCount(MAX_X);
    setRowCount(MAX_Y);

    horizontalHeader()->setDefaultSectionSize(ITEM_SIZE);   // width
    verticalHeader()->setDefaultSectionSize(ITEM_SIZE);     // height

    for(int y=0; y<MAX_Y; y++)
    {
        for(int x=0; x<MAX_X; x++)
        {
            Item_widget *item = new Item_widget();
            Q_ASSERT(item);
            item->setProperty("pos_x", x);
            item->setProperty("pos_y", y);
            connect(item, &Item_widget::item_drag,      this,   &Table_widget::item_drag);
            connect(item, &Item_widget::item_drop,      this,   &Table_widget::item_drop);
            connect(item, &Item_widget::item_update,    this,   &Table_widget::item_update);

            items[x][y].item = item;
            items[x][y].cnt  = 0;

            setCellWidget(y, x, item);
        }
    }

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    horizontalHeader()->hide();
    verticalHeader()->hide();

    setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
void Table_widget::item_drag(int x, int y, int id)
{
    qDebug() << "item_drag:" << x << y << id;

    //items[x][y].cnt = 0;

#if 0
    drag_pos_x = x;
    drag_pos_y = y;
    is_dragging = true;
#endif
}
//--------------------------------------------------------------------------------
void Table_widget::item_drop(int x, int y, int id)
{
    qDebug() << "Drop:" << x << y << id;
    items[x][y].cnt++;

#if 0
    if(is_dragging)
    {
        is_dragging = false;

        items[drag_pos_x][drag_pos_y].item->item_clear();
        items[drag_pos_x][drag_pos_y].cnt = 0;
    }
#endif

    update_inventory();
}
//--------------------------------------------------------------------------------
void Table_widget::item_update(int x, int y)
{
    int cnt = items[x][y].cnt;
    if(cnt > 0)
    {
        cnt--;
        items[x][y].cnt = cnt;
        f_play_sound();
    }
}
//--------------------------------------------------------------------------------
void Table_widget::clear_items(void)
{
    for(int y=0; y<MAX_Y; y++)
    {
        for(int x=0; x<MAX_X; x++)
        {
            items[x][y].item->setProperty("pos_x", x);
            items[x][y].item->setProperty("pos_y", y);
            items[x][y].item->setProperty("cnt", 0);
            items[x][y].item->item_clear();
        }
    }
}
//--------------------------------------------------------------------------------
void Table_widget::draw_cnt(int x, int y, int cnt)
{
//    Item_widget *item = reinterpret_cast<Item_widget *>(cellWidget(x, y));
//    Q_ASSERT(item);
//    item->draw_cnt(cnt);

//    items[x][y].item->cnt = cnt;
    items[x][y].item->draw_cnt(cnt);
}
//--------------------------------------------------------------------------------
void Table_widget::update_inventory(void)
{
    for(int y=0; y<MAX_Y; y++)
    {
        for(int x=0; x<MAX_X; x++)
        {
            int cnt = items[x][y].cnt;
            draw_cnt(x, y, cnt);
        }
    }
}
//--------------------------------------------------------------------------------
void Table_widget::f_play_sound(void)
{
    QSound::play(":/sounds/ukus-yabloka-korotkiy.wav");
}
//--------------------------------------------------------------------------------
void Table_widget::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << Q_FUNC_INFO;
}
//--------------------------------------------------------------------------------
void Table_widget::dragMoveEvent(QDragMoveEvent *event)
{
    qDebug() << Q_FUNC_INFO;
}
//--------------------------------------------------------------------------------
void Table_widget::dragLeaveEvent(QDragLeaveEvent *event)
{
    qDebug() << Q_FUNC_INFO;
}
//--------------------------------------------------------------------------------
void Table_widget::dropEvent(QDropEvent *event)
{
    qDebug() << Q_FUNC_INFO;
}
//--------------------------------------------------------------------------------
QSize Table_widget::sizeHint()
{
    return QSize(MAX_X * ITEM_SIZE,
                 MAX_Y * ITEM_SIZE);
}
//--------------------------------------------------------------------------------
