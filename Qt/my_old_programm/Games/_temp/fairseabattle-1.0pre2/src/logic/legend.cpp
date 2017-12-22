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
#include "legend.h"
//#include <iostream>

BSLegend::BSLegend(int const * const l) :
    figline(0),
    len(0)
{
//    std::cout << "BSLegend::BSLegend enter\n";
    load(l);
//    std::cout << "BSLegend::BSLegend done\n";
}

BSLegend::~BSLegend() { delete[] figline; }

void
BSLegend::raise_all() {
    for(int j(0); j<len; ++j) {
        figline[j].flag = true;
    }
}

bool
BSLegend::remove(int l) {
    for(int i(0); i < len; ++i) {
        if (figline[i].flag && figline[i].len == l) {
            figline[i].flag = false;
            return true;
        }
    }
    return false;
}

bool
BSLegend::is_empty() const {
    for(int i(0); i < len; ++i) {
        if (figline[i].flag) {
            return false;
        }
    }
    return true;
}

void
BSLegend::load(int const * const l) {
    delete[] figline;
    int const * i;
    for(i=l; *i; ++i) {};
    len = i-l;
    figline = new Figure[len];
    for(int j(0); j<len; ++j) {
        figline[j].len = l[j];
        figline[j].flag = true;
    }
}

// --------------------------------

BSLegend::iter::iter(BSLegend const & l) : p(l.figline), idx(0), limit(l.len) {}

int
BSLegend::iter::operator*() const { return p[idx].len; }

BSLegend::iter &
BSLegend::iter::operator++() {
    ++idx;
    return *this;
}

bool
BSLegend::iter::operator()() const { return idx < limit; }

// --------------------------------

void
BSLegend::uniqiter::left() {
    while(idx < limit && (p[idx].len == lastlen || !p[idx].flag)) {
//        std::cout << "BSLegend::uniqiter::left " << idx << std::endl;
        ++idx;
    }
    lastlen = p[idx].len;
}

BSLegend::uniqiter::uniqiter(BSLegend const & l) :
    p(l.figline),
    idx(0),
    limit(l.len),
    lastlen(-1)
{
    left();
}

int
BSLegend::uniqiter::operator*() const { return p[idx].len; }

BSLegend::uniqiter &
BSLegend::uniqiter::operator++() {
    ++idx;
    left();
    return *this;
}

bool
BSLegend::uniqiter::operator()() const { return idx < limit; }
