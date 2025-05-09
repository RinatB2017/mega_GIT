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
#include <QPointer>
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class Bone;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent,
            MySplashScreen *splash);
    virtual ~MainBox();

    int get_direction(int pos_x, int pos_y);

signals:
    void set_status_text(const QString &);

private slots:
    void new_game(void);
    void bone_move(int id, int pos_x, int pos_y);
    void check_move(QObject *obj);

private:
    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;

    int map[MAX_X][MAX_Y];
    Bone *bone[MAX_BONE];
    int bone_width = 0;
    int bone_height = 0;

    int cnt_move = 0;

    void init(void);
    void add_menu(void);
    void create_bones(void);
    void map_randomize(void);
    bool check_bone(int number);
    void check_win(void);

    bool check_left(int pos_x, int pos_y);
    bool check_right(int pos_x, int pos_y);
    bool check_top(int pos_x, int pos_y);
    bool check_bottom(int pos_x, int pos_y);

    void move_left(QObject *obj);
    void move_right(QObject *obj);
    void move_top(QObject *obj);
    void move_bottom(QObject *obj);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void resizeEvent (QResizeEvent * event);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
