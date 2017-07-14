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
#ifndef BS_LOGIC_GEOMETRY_H
#define BS_LOGIC_GEOMETRY_H

#include <iostream>

class BSPoint
{
friend std::ostream & operator << (std::ostream& os, const BSPoint & p);
private:
    int px;
    int py;
public:
    BSPoint();
    BSPoint(int x, int y);
    BSPoint const operator+(BSPoint const & p) const;
    BSPoint const operator-(BSPoint const & p) const;
    void operator+=(BSPoint const & p);
    void operator-=(BSPoint const & p);
    bool const operator==(BSPoint const & p) const;
    int const x() const;
    int const y() const;
    void swap();
};

class BSRect {
friend std::ostream & operator << (std::ostream& os, const BSRect & p);
private:
    BSPoint po;
    BSPoint dp;
public:
    class iter;
    class iter_inside;
    BSRect();
    BSRect(BSPoint const & a, BSPoint const & b);
    BSRect(BSPoint const & a);
    BSRect(int size);
    BSRect(int dx, int dy);
    int width() const;
    int height() const;
    int length() const;
    int square() const;
    int idx(BSPoint const & p) const;
    void swap();
    BSRect grow(BSRect const & lim) const;
    bool is_inside(BSPoint const & p) const;
};

/*
 *  Итератор всех точек в данном прямоугольнике
 */
class BSRect::iter {
private:
    BSRect rect;
    int x;
    int y;
public:
    iter(BSRect const & p);
    BSPoint operator*() const;
    BSRect::iter & operator++();
    bool operator()() const;
};

/*
 *  Итератор всех прямоугольников в данном прямоугольнике
 */
class BSRect::iter_inside {
private:
    BSRect rect;
    BSRect inside_rect;
    int x;
    int y;
public:
    iter_inside(BSRect const & p, BSRect const & q);
    BSRect operator*() const;
    BSRect::iter_inside & operator++();
    bool operator()() const;
    void swap();
};

// --------------------------------------------------------

std::ostream & operator << (std::ostream& os, const BSRect & p);
std::ostream & operator << (std::ostream& os, const BSPoint & p);

#endif
