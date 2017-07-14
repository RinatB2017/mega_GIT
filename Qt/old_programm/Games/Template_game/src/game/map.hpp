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
#ifndef MAP_HPP
#define MAP_HPP
//--------------------------------------------------------------------------------
class QString;
//--------------------------------------------------------------------------------
#define MAX_X 1024
#define MAX_Y 1024
//--------------------------------------------------------------------------------
enum {
    NO_ERROR_ADV = 0,
    ERROR_WIDTH,
    ERROR_HEIGHT,
    ERROR_FILE_NOT_OPEN,
    ERROR_FILE_SIZE
};
enum {
    MAP_ZERO = 0,
    MAP_BALL = 1,
    MAP_FIELD = 2,
    MAP_PLAYER = 3,
    MAP_FOOD = 4,
    MAP_WALL = 5,
    MAP_DISK_RED = 6,
    MAP_DISK_YELLOW = 7,
    MAP_DISK_ORANGE = 8,
    MAP_MONITOR = 9,
    MAP_ALIEN = 10,
    MAP_EXIT_CLOSED = 11,
    MAP_EXIT_OPENED = 12
};
//--------------------------------------------------------------------------------
class Map
{
public:
    Map(void);

    void init(void);

    int load(const QString &filename);

    void set_max_x(int value);
    void set_max_y(int value);

    int get_max_x(void);
    int get_max_y(void);

    int  set(int x, int y, char value);
    char get(int x, int y);

private:
    char map[MAX_X][MAX_Y];
    int width;
    int height;
};
//--------------------------------------------------------------------------------
#endif
