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
#ifndef BS_LOGIC_LEGEND_H
#define BS_LOGIC_LEGEND_H

#include <iostream>

class BSLegend {
friend std::ostream & operator << (std::ostream& os, const BSLegend & l);
private:
    struct Figure {
        int len;
        bool flag;
    } * figline;
    int len;
public:
    class iter;
    class uniqiter;
    BSLegend(int const * const l);
    BSLegend(const BSLegend &); /// IMPLEMENT IT!!
    ~BSLegend();

    BSLegend & operator=(const BSLegend &); /// IMPLEMENT IT!!
    BSLegend * operator&(); /// IMPLEMENT IT!?
    BSLegend const * operator&() const; /// IMPLEMENT IT!?

    void raise_all();
    bool remove(int l);
    bool is_empty() const;
    void load(int const * const l);
};

class BSLegend::iter {
private:
    Figure *p;
    int idx;
    int limit;
public:
    iter(BSLegend const & l);
    int operator*() const;
    BSLegend::iter & operator++();
    bool operator()() const;
};

class BSLegend::uniqiter {
private:
    Figure *p;
    int idx;
    int limit;
    int lastlen;

    void left();
public:
    uniqiter(BSLegend const & l);
    int operator*() const;
    BSLegend::uniqiter & operator++();
    bool operator()() const;
};

#endif
