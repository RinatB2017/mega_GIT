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
#include <QMessageBox>
#include <QPushButton>
#include <QToolButton>
#include <QToolBar>
#include <QLabel>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "defines.hpp"
#include "mainbox.hpp"
//--------------------------------------------------------------------------------
#include "bone.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    cnt_move = 0;

    add_menu();
    create_bones();
    new_game();

    setMinimumSize(BONE_SIZE * MAX_X, BONE_SIZE * MAX_Y);
}
//--------------------------------------------------------------------------------
void MainBox::add_menu(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw == nullptr)
    {
        return;
    }

    QAction *action = new QAction(this);
    action->setText("new game");
    action->setIcon(QIcon(ICON_PROGRAMM));
    connect(action, SIGNAL(triggered(bool)), this, SLOT(new_game()));

    bool ok = mw->add_action(mw->get_file_menu(), 0, action);
    if(ok)
    {
        mw->add_separator(mw->get_file_menu(), 1);
    }
}
//--------------------------------------------------------------------------------
bool MainBox::check_bone(int number)
{
    if(number == 0)
    {
        //пустое пространство
        return false;
    }

    for(int n=0; n<(MAX_X*MAX_Y); n++)
    {
        if(map[n / MAX_Y][n % MAX_X] == number)
        {
            //такие кости уже были
            return false;
        }
    }
    //таких костей нет
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::map_randomize(void)
{
    bool ok = false;
    int bone = 0;
    for(int n=0; n<(MAX_X*MAX_Y); n++)
    {
        map[n / MAX_Y][n % MAX_X] = 0;
    }
    for(int n=0; n<((MAX_X*MAX_Y) - 1); n++)
    {
        ok = false;
        while(!ok)
        {
            bone = rand() % 16;
            ok = check_bone(bone);
        }
        map[n / MAX_Y][n % MAX_X] = bone;
    }
}
//--------------------------------------------------------------------------------
void MainBox::new_game(void)
{
    map_randomize();

    for(int n=0; n<MAX_BONE; n++)
    {
        int property_id = map[n / MAX_Y][n % MAX_X];
        int y = n % MAX_X;
        int x = n / MAX_Y;
        bone[n]->setNumber(property_id);
        bone[n]->setProperty("property_id", property_id);
        bone[n]->setProperty("property_x", x);
        bone[n]->setProperty("property_y", y);
        //map[x][y] = property_id;
        bone[n]->move(x*bone_width, y*bone_height);

        bone[n]->setEnabled(true);
    }
    cnt_move = 0;
    emit set_status_text(QString("%1").arg(cnt_move));
}
//--------------------------------------------------------------------------------
void MainBox::create_bones(void)
{
    bone_width  = BONE_SIZE;
    bone_height = BONE_SIZE;

    for(int n=0; n<MAX_BONE; n++)
    {
        bone[n] = new Bone(this);
        bone[n]->installEventFilter(this);
    }
}
//--------------------------------------------------------------------------------
void MainBox::bone_move(int property_id, int pos_x, int pos_y)
{
    for(int y=0; y<MAX_Y; y++)
    {
        for(int x=0; x<MAX_X; x++)
        {
            if(map[x][y] == property_id)
            {
                map[x][y] = 0;
            }
        }
    }
    map[pos_x][pos_y] = property_id;

    check_win();
}
//--------------------------------------------------------------------------------
void MainBox::check_win(void)
{
    int n = 1;
    for(int y=0; y<MAX_Y; y++)
    {
        for(int x=0; x<MAX_X; x++)
        {
            if((map[x][y] != n) && (map[x][y] != 0))
            {
                return;
            }
            n++;
        }
    }

    //---
    QMessageBox::information(this, tr("Победа!"), tr("Вы выиграли!"));
    for(int n=0; n<MAX_BONE; n++)
    {
        bone[n]->setEnabled(false);
    }
}
//--------------------------------------------------------------------------------
int MainBox::get_direction(int pos_x, int pos_y)
{
    bool ok = false;

    ok = check_left(pos_x, pos_y);
    if(ok) return LEFT;
    ok = check_right(pos_x, pos_y);
    if(ok) return RIGHT;
    ok = check_top(pos_x, pos_y);
    if(ok) return TOP;
    ok = check_bottom(pos_x, pos_y);
    if(ok) return BOTTOM;

    return NONE;
}
//--------------------------------------------------------------------------------
bool MainBox::check_left(int pos_x, int pos_y)
{
    if(pos_x == 0) return false;
    if(map[pos_x-1][pos_y] != 0) return false;

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::check_right(int pos_x, int pos_y)
{
    if(pos_x == (MAX_X - 1)) return false;
    if(map[pos_x+1][pos_y] != 0) return false;

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::check_top(int pos_x, int pos_y)
{
    if(pos_y == 0) return false;
    if(map[pos_x][pos_y-1] != 0) return false;

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::check_bottom(int pos_x, int pos_y)
{
    if(pos_y == (MAX_Y - 1)) return false;
    if(map[pos_x][pos_y+1] != 0) return false;

    return true;
}
//--------------------------------------------------------------------------------
void MainBox::resizeEvent(QResizeEvent *event)
{
    bone_width  = event->size().width() / MAX_X;
    bone_height = event->size().height() / MAX_Y;

    for(int n=0; n<MAX_BONE; n++)
    {
        int x = bone[n]->property("property_x").toInt();
        int y = bone[n]->property("property_y").toInt();
        bone[n]->move(x*bone_width, y*bone_height);
        bone[n]->resize(bone_width, bone_height);
    }
}
//--------------------------------------------------------------------------------
void MainBox::move_left(QObject *obj)
{
    int property_id = obj->property("property_id").toInt();
    int x  = obj->property("property_x").toInt() * bone_width;
    int y  = obj->property("property_y").toInt() * bone_height;

    x -= bone_width;
    if(x < 0) return;

    obj->setProperty("property_x", x / bone_width);
    obj->setProperty("property_y", y / bone_height);
    QWidget *bone = (QWidget *)obj;
    bone->move(x, y);
    bone_move(property_id, x / bone_width, y / bone_height);

    cnt_move++;
    emit set_status_text(QString("%1").arg(cnt_move));
}
//--------------------------------------------------------------------------------
void MainBox::move_right(QObject *obj)
{
    int property_id = obj->property("property_id").toInt();
    int x = obj->property("property_x").toInt() * bone_width;
    int y = obj->property("property_y").toInt() * bone_height;

    x += bone_width;
    if(x >= (bone_width * MAX_X)) return;

    obj->setProperty("property_x", x / bone_width);
    obj->setProperty("property_y", y / bone_height);
    QWidget *bone = (QWidget *)obj;
    bone->move(x, y);
    bone_move(property_id, x / bone_width, y / bone_height);

    cnt_move++;
    emit set_status_text(QString("%1").arg(cnt_move));
}
//--------------------------------------------------------------------------------
void MainBox::move_top(QObject *obj)
{
    int property_id = obj->property("property_id").toInt();
    int x = obj->property("property_x").toInt() * bone_width;
    int y = obj->property("property_y").toInt() * bone_height;

    y -= bone_height;
    if(y < 0) return;

    obj->setProperty("property_x", x / bone_width);
    obj->setProperty("property_y", y / bone_height);
    QWidget *bone = (QWidget *)obj;
    bone->move(x, y);
    bone_move(property_id, x / bone_width, y / bone_height);

    cnt_move++;
    emit set_status_text(QString("%1").arg(cnt_move));
}
//--------------------------------------------------------------------------------
void MainBox::move_bottom(QObject *obj)
{
    int property_id = obj->property("property_id").toInt();
    int x = obj->property("property_x").toInt() * bone_width;
    int y = obj->property("property_y").toInt() * bone_height;

    y += bone_height;
    if(y >= (bone_height * MAX_Y)) return;

    obj->setProperty("property_x", x / bone_width);
    obj->setProperty("property_y", y / bone_height);
    QWidget *bone = (QWidget *)obj;
    bone->move(x, y);
    bone_move(property_id, x / bone_width, y / bone_height);

    cnt_move++;
    emit set_status_text(QString("%1").arg(cnt_move));
}
//--------------------------------------------------------------------------------
void MainBox::check_move(QObject *obj)
{
    int x  = obj->property("property_x").toInt();
    int y  = obj->property("property_y").toInt();

    int dir = get_direction(x, y);
    switch(dir)
    {
    case LEFT:
        move_left(obj);
        break;

    case RIGHT:
        move_right(obj);
        break;

    case TOP:
        move_top(obj);
        break;

    case BOTTOM:
        move_bottom(obj);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
bool MainBox::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        check_move(obj);
        return true;
    }
    return QObject::eventFilter(obj, event);
}
//--------------------------------------------------------------------------------
void MainBox::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
