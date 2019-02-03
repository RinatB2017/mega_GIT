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
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class QComboBox;
class QCheckBox;
class QToolButton;
class QToolBar;
class QLCDNumber;
class World;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent = 0);
    ~MainBox();

private slots:
    void choice_test(void);
    void test(void);

private:
    struct CMD
    {
        int cmd;
        QString cmd_text;
        void (World::*func)(void);
    };

    Ui::MainBox *ui;
    World *world;
    QLCDNumber *lcd;
    QCheckBox *cb_block_insert_object;

    QComboBox *cb_test;
    QList<CMD> commands;

    void init(void);

    void createTestBar(void);

    void load_setting(void);
    void save_setting(void);

    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
