/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#ifndef MAP_HPP
#define MAP_HPP
//--------------------------------------------------------------------------------
#include <QGridLayout>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class Map : public MyWidget
{
    Q_OBJECT

public:
    Map(QWidget *parent = 0);
    ~Map();

    void new_map(int max_x, int max_y);
    bool load_map(const QString &filename);
    bool save_map(const QString &filename);

    int rowCount(void);
    int columnCount(void);

    bool find_start(int *x, int *y);
    bool find_player(int *x, int *y);

    void player_move_up(void);
    void player_move_down(void);
    void player_move_left(void);
    void player_move_right(void);

    int get_id(int x, int y);
    bool add_item(int x, int y, int id);
    bool put_picture(int id, int x, int y);

signals:
    void move_to(int x, int y);

public slots:
    bool start(unsigned int interval_ms);
    void stop(void);
    void refresh(void);

    void set_cursor(void);

    void update(void);

private:
    QGridLayout *grid_map = 0;

    int id_map[MAX_WIDTH][MAX_HEIGHT];

    quint64 cnt_move = 0;

    int max_x = 0;
    int max_y = 0;

    int id = 0;
    int direction_move = 0;
    int player_x = 0;
    int player_y = 0;
    int start_x = 0;
    int start_y = 0;
    QTimer *timer = 0;

    void init(void);
    void init_id_map(void);
    void createTimer(void);

    QPixmap rotate(const QString &filename, int angle);

    void updateText(void);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};
//--------------------------------------------------------------------------------
#endif // MAP_HPP
