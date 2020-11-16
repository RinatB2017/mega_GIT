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
#ifndef ITEM_HPP
#define ITEM_HPP
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class Item;
}
//--------------------------------------------------------------------------------
class Item : public MyWidget
{
    Q_OBJECT

signals:
    void remove_item(int);

public:
    explicit Item(QListWidget *lw, QWidget *parent = nullptr);
    ~Item();

    void set_pixmap(const QPixmap &pixmap);
    void led_on(void);
    void led_off(void);

private:
    Ui::Item *ui;
    QListWidget *parent_lw = nullptr;

    QPixmap pixmap;

    void init(void);
    void item_close(void);
    void item_click(void);
    void item_load(void);
    void item_save(void);
    void item_info(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // ITEM_HPP
