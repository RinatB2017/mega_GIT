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
#include "arena.h"
//#include "map_io.h"
#include <assert.h>

//--------------------------------------

class AllValidFiguresIter {
private:
    BSMap<BSFlags> *arena_map;
    int fig_len;
    int dir;
    BSRect::iter_inside iter_inside;

    void incr() {
        ++iter_inside;
        if (iter_inside()) return;
        ++dir;
        iter_inside.swap();
    }
    void left() {
        while((*this)()) {
            bool can = true;
            for(BSRect::iter p(*iter_inside); p(); ++p) {
//              std::cout << " -  " << *p << " : " << (*arena_map)[*p]->marked() << std::endl;
                if ((*arena_map)[*p]->marked()) {
                    can = false;
                    break;
                }
            }
            if (can) return;
            incr();
        }
    }
public:
    AllValidFiguresIter(BSMap<BSFlags> *m, int l):
        arena_map(m),
        fig_len(l),
        dir(0),
        iter_inside(m->get_rect(), BSRect(1, l))
    {
        left();
    }
    BSRect operator*() const {
        return *iter_inside;
    }
    AllValidFiguresIter & operator++() {
        incr();
        left();
        return *this;
    }
    bool operator()() const {
        return dir < 2;
    }
};

//--------------------------------------

void
BSArena::search(BSPoint & p, BSPoint const & dp) const {
    for (;arena_map.get_rect().is_inside(p); p += dp) {
        if (! arena_map[p]->occupied()) { break; }
    }
    p -= dp;
}

BSRect
BSArena::search_rect(BSPoint const & fr) const {
    BSPoint p1(fr);
    search(p1, BSPoint(0, -1));
    search(p1, BSPoint(-1, 0));
    BSPoint p2(fr);
    search(p2, BSPoint(0, 1));
    search(p2, BSPoint(1, 0));
    BSRect fired_area(p1, p2-p1+BSPoint(1, 1));
    return fired_area;
}

//-----

BSArena::BSArena(int sz, int const * const l):
    arena_map(sz),
    arena_legend(l)
{
//    std::cout << "BSArena::BSArena() done" << std::endl;
}

BSFlags &
BSArena::operator[](BSPoint const & p) const {
    return *arena_map[p];
}

BSRect
BSArena::get_rect() const {
    return arena_map.get_rect();
}

/*
void
BSArena::reinit(int const * const l) {
    arena_legend.load(l);
    for(BSRect::iter p(arena_map.get_rect()); p(); ++p) {
        arena_map[*p]->clean();
    }
}
*/

void
BSArena::clean() {
    for(BSRect::iter p(arena_map.get_rect()); p(); ++p) {
        arena_map[*p]->clean();
    }
}

void
BSArena::load_legend(int const * const l) {
    arena_legend.load(l);
}

// --------------------------------------------

BSArenaPuzzle::BSArenaPuzzle(int sz, int const * const l) :
    BSArena(sz, l),
    counts(arena_map.get_rect()),
    already_finded(0) {}

BSPoint
BSArenaPuzzle::find_fire() {
//    std::cout  << "find fire" << std::endl
//               << arena_map << std::endl;
    // ��������� �ޣ�����
    if (already_finded == 0) {
        // �������� �ޣ�����
        for(BSRect::iter p(counts.get_rect()); p(); ++p) {
            *counts[*p] = 0;
        }
//        std::cout << "in search mode" << std::endl;
//        std::cout << arena_legend << std::endl;
        for (BSLegend::uniqiter l(arena_legend); l(); ++l) {
            for(AllValidFiguresIter i(&arena_map, *l); i(); ++i) {
                for(BSRect::iter p(*i); p(); ++p) {
                    ++*counts[*p];
                }
            }
//            std::cout << "len=" << *l << std::endl;
        }
    } else {
//        std::cout << " ========== MEGASEARCH ========= (" << already_finded << ")" << std::endl;
        int mx_cn(0);
        for (BSLegend::uniqiter l(arena_legend); l(); ++l) {
/* �� ���������� �������� �������
            if (*l <= already_finded) {
                continue;
            }
*/
//            std::cout << "len=" << *l << std::endl;
            for(AllValidFiguresIter i(&arena_map, *l); i(); ++i) {
                // ��������� ������� �� ����������
                // - � ���� ���� ��������� ����
                // - � ��������� ��� ��������� �����
                int cn(0);
                for(BSRect::iter p(*i); p(); ++p) {
                    if (arena_map[*p]->occupied()) {
                        ++cn;
                    }
                }
                if (cn == 0) { // ���� ������� ����� ��� -- ��� ��� �� �����
                    continue;
                }
                int cnn(0);
                for(BSRect::iter p((*i).grow(arena_map.get_rect())); p(); ++p) {
                    if (arena_map[*p]->occupied()) {
                        ++cnn;
                    }
                }
                if (cnn != cn) { // ���� ���� ������� � ��������� -- ��� ��� �� �����
                    continue;
                }
                if (mx_cn < cn) { // ���� ��� ����� ������ -- �������� �ޣ�����
                    for(BSRect::iter p(counts.get_rect()); p(); ++p) {
                        *counts[*p] = 0;
                    }
                    mx_cn = cn;
                }
                if (mx_cn == cn) { // ��������� ������������
                    for(BSRect::iter p(*i); p(); ++p) { // copypast?
                        if (! arena_map[*p]->fired()) {
                            ++*counts[*p];
                        }
                    }
                }
            }
        }
    }
//    std::cout << counts << std::endl;
    // ������� ������������
    BSPoint mp[arena_map.get_rect().square()];
    int m = -1;
    int c = 0;
    for(BSRect::iter p(counts.get_rect()); p(); ++p) {
        if (*counts[*p] == m) {
            mp[c] = *p;
            c++;
        } else if (*counts[*p] > m) {
            mp[0] = *p;
            c = 1;
            m = *counts[*p];
        }
    }
//    std::cout << m << std::endl
//              << "/find fire" << std::endl;
    return mp[rand() % c];
}

// ��� ������� ���������� �������, ������� ���� ��������
// ����� ����, ��� ��ģ� �ޣ� ������� �������, ���
// �������� ������� find_fire
// ������ ����������� ������������ � ������������
// ����������; ������ -- � ��������.
// ��������, ��� ���� ���-�� ���������... ��� �����?
BSRect
BSArenaPuzzle::apply_result(BSPoint const & fire, BSArena::result const result) {
    arena_map[fire]->fire();
//    std::cout << arena_map << std::endl;
    switch(result) {
        case BSArena::r_milk:
            arena_map[fire]->mark();
            return BSRect(fire);
        case BSArena::r_already_fired:
            return BSRect(fire);
        case BSArena::r_wasted_effort:
            return BSRect(fire);
        case BSArena::r_continue_in_that_direction:
            already_finded += 1; // success
            arena_map[fire]->occupy();
            return BSRect(fire);
        case BSArena::r_drowned:
        case BSArena::r_game_over:
            already_finded += 1; // success
            arena_map[fire]->occupy();
            BSRect fired_area = search_rect(fire);
            // �������� ������ �������, ������� �������� ������,
            // ��� �������� ����� �� ���, ������� ��������
            already_finded -= fired_area.length();
            BSRect updated_urea = fired_area.grow(arena_map.get_rect());
            for(BSRect::iter p(updated_urea); p(); ++p) {
                arena_map[*p]->mark();
            }
            arena_legend.remove(fired_area.length());
            return updated_urea;
    }
    assert(false); // ����������� ��������!
    return BSRect();
}

void
BSArenaPuzzle::clean() {
    BSArena::clean();
    already_finded = 0;
}

// -----------------------------------------------

BSArenaKey::BSArenaKey(int sz, int const * const l) : BSArena(sz, l) {}

bool
BSArenaKey::has_not_fired_neighbor(BSRect const & fired_area) const {
    // �������, ��� �� �� ������� ������� ������� ��-������� ����������
    for(BSRect::iter p(fired_area.grow(arena_map.get_rect())); p(); ++p) {
        if (arena_map[*p]->occupied() && ! arena_map[*p]->fired()) {
            return true; // �ݣ ���� ���� ��������
        }
    }
    return false;
}

// ������� �������� ����� �������� ����,
// ����� ��������������� �� �������.
// ������, ������ ��� ����� �� ������,
// �� �ӣ ����� �� ����� ���� const, ��� ���
// ���������� � ���������� �������.
// ��� �� ����� �������. ���� ���� ���������
// ��������, ���� ������������ ����� �������.
bool
BSArenaKey::check() {
//    std::cout << "BSArenaKey::check()" << std::endl;
    bool setup_is_ok(true);
    // ����� �� ���� ������
    for(BSRect::iter p(arena_map.get_rect()); p(); ++p) {
        // ������������� �����, � ������� ���� ������,
        // �� ������� �ݣ �� �������� (������� ������������
        // ��� ����, ����� �� ������������� ���� ������ ������)
        if (arena_map[*p]->occupied() &&
            !arena_map[*p]->fired()) {
            // ���� ������
            BSRect fired_area = search_rect(*p);
//          std::cout << "fired_area = " << fired_area << std::endl;
            // ��������, ����� �� ������������� ������ ���
            for(BSRect::iter i(fired_area); i(); ++i) {
                arena_map[*i]->fire();
            }
//          std::cout << arena_map << std::endl;
            // �������, ���� �� � ������ ��-����������
            if (has_not_fired_neighbor(fired_area)) {
//                std::cout << "ERROR!!" << std::endl;
                setup_is_ok = false;
                break;
            }
            // ��������� ������������ �������
            if (!arena_legend.remove(fired_area.length())) {
//                std::cout << "ERROR!! (2)" << std::endl;
                setup_is_ok = false;
                break;
            }
        }
    }
    // ��������� ��� �� ������� ������������
    if (setup_is_ok) {
        setup_is_ok = arena_legend.is_empty();
    }
    // ������ �������
    arena_legend.raise_all();
    for(BSRect::iter p(arena_map.get_rect()); p(); ++p) {
        arena_map[*p]->unfire();
    }
//    std::cout << arena_map << std::endl;
    return setup_is_ok;
}

void
BSArenaKey::auto_setup() {
    clean();
    for (BSLegend::iter l(arena_legend); l(); ++l) {
        int c(0);
        BSRect all_valid[2 * arena_map.get_rect().square()];
        for(AllValidFiguresIter i(&arena_map, *l); i(); ++i) {
            all_valid[c] = *i;
            ++c;
        }
        assert(c > 0);
        int i(rand() % c);
        for(BSRect::iter p(all_valid[i]); p(); ++p) {
            arena_map[*p]->occupy();
        }
        for(BSRect::iter p(all_valid[i].grow(arena_map.get_rect())); p(); ++p) {
            arena_map[*p]->mark();
        }
    }
//    std::cout << "BSArena::auto_setup()\n" << arena_map << std::endl;
    for(BSRect::iter p(arena_map.get_rect()); p(); ++p) {
        arena_map[*p]->unmark();
    }
}

BSArena::result
BSArenaKey::apply_fire(BSPoint const & fr) {
    if (arena_map[fr]->fired()) {
        return BSArena::r_already_fired; // ���
    }
    arena_map[fr]->fire();
    if (arena_map[fr]->marked()) {
        return BSArena::r_wasted_effort; // � ��� ����, ��� ������
    }
    if (arena_map[fr]->occupied()) { // ��� ������
        // ����������� ������ ��
        // ������� ������� �������
        BSRect fired_area = search_rect(fr);
        // �������, ��� �� �� ������� ������� ������� ��-������� ����������
        if (has_not_fired_neighbor(fired_area)) {
            return BSArena::r_continue_in_that_direction;
        }
        arena_legend.remove(fired_area.length());
//        std::cout << arena_legend << " (aft.rem)" << std::endl;
        if (arena_legend.is_empty()) {
            return BSArena::r_game_over; // �� �ӣ ������
        }
        return BSArena::r_drowned; // �ӣ. �����
    }
    return BSArena::r_milk; // � ������
}
