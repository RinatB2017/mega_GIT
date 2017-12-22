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
#ifndef BS_LOGIC_ARENA_H
#define BS_LOGIC_ARENA_H

#include "flags.h"
#include "legend.h"
#include "map.h"

class BSArena {
friend std::ostream & operator << (std::ostream& os, const BSArena & a);
protected:
    BSMap<BSFlags> arena_map;
    BSLegend arena_legend;

    void search(BSPoint & p, BSPoint const & dp) const;
    BSRect search_rect(BSPoint const & fr) const;

public:
    enum result {
        r_milk,            // мимо
        r_already_fired,   // сюда уже стреляли
        r_wasted_effort,   // мимо, но вам и так должно быть известно, что здесь ничего нет
        r_continue_in_that_direction, // ранен, продолжайте добивать
        r_drowned,         // утонул
        r_game_over        // добит последний, игра окончена
    };
    BSArena(int sz, int const * const l);
    BSFlags & operator[](BSPoint const & p) const;
    BSRect get_rect() const;
    void load_legend(int const * const l);
    void clean();
    // если понадобится деструктор, то он должен быть виртуальным!
};

class BSArenaPuzzle : public BSArena {
private:
    BSMap<int> counts;
    int already_finded;

public:
    BSArenaPuzzle(int sz, int const * const l);
    BSPoint find_fire();
    BSRect apply_result(BSPoint const & fr, BSArena::result const result);
    void clean();
};

class BSArenaKey : public BSArena {
private:
    bool has_not_fired_neighbor(BSRect const & fired_area) const;

public:
    BSArenaKey(int sz, int const * const l);
    void auto_setup(); // <-- move it to BSArenaKey !!!
    bool check();
    BSArena::result apply_fire(BSPoint const & fr);
};

#endif
