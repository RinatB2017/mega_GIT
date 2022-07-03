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
#ifndef MAINBOX_GUI_HPP
#define MAINBOX_GUI_HPP
//--------------------------------------------------------------------------------
#include "ui_for_tests_mainbox_gui.h"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox_GUI;
}
//--------------------------------------------------------------------------------
class MainBox_GUI : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox_GUI(QWidget *parent);
    virtual ~MainBox_GUI();

private:
    Ui::MainBox_GUI *ui;

    typedef struct ITEM
    {
        QTreeWidgetItem *parent_item;
        QTreeWidgetItem *item;
        QString caption;
        Qt::CheckState state;
        void (MainBox_GUI::*func)(void);
    } *item_t;
    QList<ITEM> items;

    void init(void);
    void updateText(void);

    QTreeWidgetItem *root_item = nullptr;
    void set_item_param(QTreeWidgetItem *item,
                        Qt::ItemFlags flags,
                        const QString &caption,
                        Qt::CheckState state);
    void set_children_state(QTreeWidgetItem *parent_item,
                            Qt::CheckState state);
    void f_not_created(void);

    void btn_1_clicked(void);
    void btn_2_clicked(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_GUI_HPP
