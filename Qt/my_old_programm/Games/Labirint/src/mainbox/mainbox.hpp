/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#include <QList>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class QToolButton;
class QToolBar;
class QComboBox;
class QSpinBox;
class QFrame;
//--------------------------------------------------------------------------------
enum {
    UP = 0,
    DOWN,
    LEFT,
    RIGHT
};
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    ~MainBox();

private slots:
    void start(void);
    void stop(void);
    void update(void);

    void new_map(void);
    void load_map(void);
    void save_map(void);

    void set_cursor(void);

private:
    MySplashScreen *splash = 0;
    Ui::MainBox *ui = 0;

    QComboBox *cb_test = 0;

    int id = 0;
    int direction_move = 0;
    int player_x = 0;
    int player_y = 0;
    QSpinBox *sb_interval = 0;
    QTimer *timer = 0;

    void init(void);

    void new_map(int max_x, int max_y);
    bool load_map(const QString &filename);
    bool save_map(const QString &filename);

    void put_picture(int id, int x, int y);
    int  get_picture_id(int x, int y);

    void createTimer(void);
    void createTestBar(void);
    void createImagesDock(void);

    bool find_player(void);

    QPixmap rotate(const QString &filename, int angle);

    void player_move_up(void);
    void player_move_down(void);
    void player_move_left(void);
    void player_move_right(void);

    QToolButton *create_button(const QString &name, int id);

    void block_this_button(bool state);
    void block_interface(bool state);

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);
    virtual void changeEvent(QEvent *event);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
