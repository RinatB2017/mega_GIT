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
#include "arena_io.h"
#include "map_io.h"
#include "legend_io.h"

std::ostream & operator << (std::ostream& os, const BSArena & a) {
    os << "BSArena()" << std::endl <<
           a.arena_legend << std::endl <<
           a.arena_map.get_rect() << std::endl <<
           a.arena_map ; // << std::endl;
    return os;
}
