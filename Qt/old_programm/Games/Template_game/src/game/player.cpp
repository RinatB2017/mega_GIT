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
#include "player.hpp"
#include "game.hpp"
#include "map.hpp"
//--------------------------------------------------------------------------------
Player::Player(Map *map, bool is_alive, int pos_x, int pos_y)
{
    this->map = map;

    alive = is_alive;
    this->pos_x = pos_x;
    this->pos_y = pos_y;

    init();
}
//--------------------------------------------------------------------------------
bool Player::is_alive(void)
{
    return alive;
}
//--------------------------------------------------------------------------------
int Player::get_x(void)
{
    return pos_x;
}
//--------------------------------------------------------------------------------
int Player::get_y(void)
{
    return pos_y;
}
//--------------------------------------------------------------------------------
void Player::top(void)
{
    if(map->get(pos_x, pos_y-1) != 1)
    {
        map->set(pos_x, pos_y, SPACE);
        pos_y--;
        map->set(pos_x, pos_y, PLAYER);
    }
}
//--------------------------------------------------------------------------------
void Player::bottom(void)
{
    if(map->get(pos_x, pos_y+1) != 1)
    {
        map->set(pos_x, pos_y, SPACE);
        pos_y++;
        map->set(pos_x, pos_y, PLAYER);
    }
}
//--------------------------------------------------------------------------------
void Player::left(void)
{
    if(map->get(pos_x-1, pos_y) != 1)
    {
        map->set(pos_x, pos_y, SPACE);
        pos_x--;
        map->set(pos_x, pos_y, PLAYER);
    }
}
//--------------------------------------------------------------------------------
void Player::right(void)
{
    if(map->get(pos_x+1, pos_y) != 1)
    {
        map->set(pos_x, pos_y, SPACE);
        pos_x++;
        map->set(pos_x, pos_y, PLAYER);
    }
}
//--------------------------------------------------------------------------------
void Player::init(void)
{

}
//--------------------------------------------------------------------------------
