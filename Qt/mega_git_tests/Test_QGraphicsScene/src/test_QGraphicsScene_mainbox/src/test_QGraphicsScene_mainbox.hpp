/*********************************************************************************
**                                                                              **
**     Copyright (C) 2023                                                       **
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
#include <QGraphicsRectItem>
#include <QGraphicsScale>
#include <QBrush>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
#include "item.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    virtual ~MainBox();

private slots:
    void test();

private:
    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;

    bool is_blocked = false;

    //TODO не надо делать QPointer
    QGraphicsScene *scene = nullptr;
    QGraphicsItem *text = nullptr;

    Item *rectangle_R = nullptr;
    Item *rectangle_G = nullptr;
    Item *rectangle_B = nullptr;

    void init();

    void createTestBar();
    void block_interface(bool state);

    void updateText();
    bool programm_is_exit();
    void load_setting();
    void save_setting();
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
