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
#ifndef TABLE_WIDGET_HPP
#define TABLE_WIDGET_HPP
//--------------------------------------------------------------------------------
#include <QDragEnterEvent>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QDebug>
#include <QSize>

#ifdef USE_SOUND
#   include <QSound>
#endif

#include "item_widget.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
QT_BEGIN_NAMESPACE
    class QMimeData;
QT_END_NAMESPACE
//--------------------------------------------------------------------------------
typedef struct ITEM
{
    Item_widget *item;
    int cnt = 0;
} *item_t;
//--------------------------------------------------------------------------------
class Table_widget : public QTableWidget
{
    Q_OBJECT

public:
    explicit Table_widget(QWidget *parent = nullptr);
    ~Table_widget();

    void draw_cnt(int x, int y, int cnt);
    void clear_items(void);

private:
    void init(void);

    ITEM items[MAX_X][MAX_Y];

    bool is_dragging = false;
    int drag_pos_x = 0;
    int drag_pos_y = 0;

    void item_drag(int x, int y, int id);
    void item_drop(int x, int y, int id);
    void item_update(int x, int y);
    void update_inventory(void);
    void f_play_sound(void);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *) override;
    void dropEvent(QDropEvent *event) override;

    QSize sizeHint();
};
//--------------------------------------------------------------------------------
#endif // TABLE_WIDGET_HPP
