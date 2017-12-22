/*
 * Copyright 2010 Alexey Michurin
 *
 * This file is part of FairSeaBattle.
 *
 * FairSeaBattle is free software: you can redistribute
 * it and/or modify it under the terms of the
 * GNU General Public License as published by
 * the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * FairSeaBattle is distributed in the hope that
 * it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the
 * GNU General Public License along with FairSeaBattle.
 * If not, see <http://www.gnu.org/licenses/>.
 */
#include "flags_colors.h"

const QColor ColorStrategy::flag_to_color_map[] = {
    QColor(102,102,102), // 0 пусто
    QColor(  0,204,  0), // 1 живой корабль
    QColor(255,153,153), // 2 мимо (но не используется?)
    QColor(250,  0,  0), // 3 ранен
    QColor( 51,102,153), // 4 сюда можно не стрелять
    QColor(255, 60, 20), // 5 (не используется?)
    QColor( 76,179,250), // 6 мимо
    QColor(204,204,  0)  // 7 потоплен
};

ColorStrategy::ColorStrategy() :
    target_color(flag_to_color_map[0]), // XXX
    update_mode(update_mode_smooth)
{
}


ColorStrategy::ColorStrategy(BSFlags const & f, const ColorStrategyMode & b) :
    target_color(flag_to_color_map[(f.occupied() ? 1 : 0) +
                                   (f.fired()    ? 2 : 0) +
                                   (f.marked()   ? 4 : 0)]),
    update_mode(b)
{
}

QColor
ColorStrategy::color() const {
    return target_color;
}

ColorStrategyMode
ColorStrategy::mode() const {
    return update_mode;
}
