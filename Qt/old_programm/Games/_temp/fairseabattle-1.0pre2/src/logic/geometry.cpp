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
#include "geometry.h"

#include <assert.h>

// ----------------------

BSPoint::BSPoint() : px(0), py(0) {}

BSPoint::BSPoint(int x, int y): px(x), py(y) {}

BSPoint const
BSPoint::operator+(BSPoint const & p) const {
    return BSPoint(px + p.px, py + p.py);
}

BSPoint const
BSPoint::operator-(BSPoint const & p) const {
    return BSPoint(px - p.px, py - p.py);
}

void
BSPoint::operator+=(BSPoint const & p) {
    px+=p.px;
    py+=p.py;
}

void
BSPoint::operator-=(BSPoint const & p) {
    px-=p.px;
    py-=p.py;
}

bool const
BSPoint::operator==(BSPoint const & p) const {
    return (px == p.px && py == p.py);
}

int const
BSPoint::x() const { return px; }

int const
BSPoint::y() const { return py; }

void
BSPoint::swap() { int t = px; px = py; py = t; }

// ----------------------

BSRect::BSRect(): po(0, 0), dp(0, 0) {}

BSRect::BSRect(BSPoint const & a, BSPoint const & b): po(a), dp(b) {}

BSRect::BSRect(BSPoint const & a): po(a), dp(1, 1) {}

BSRect::BSRect(int size): po(0, 0), dp(size, size) {}

BSRect::BSRect(int dx, int dy): po(0, 0), dp(dx, dy) {}

int
BSRect::width() const { return dp.x(); }

int
BSRect::height() const { return dp.y(); }

int
BSRect::length() const { return dp.x() > dp.y() ? dp.x() : dp.y(); }

int
BSRect::square() const { return width()*height(); }

int
BSRect::idx(BSPoint const & p) const {
    BSPoint rel(p-po);
    int i = rel.x() + rel.y()*width();
    assert(i >= 0 and i < square());
    return i;
}

void
BSRect::swap() { dp.swap(); }

BSRect
BSRect::grow(BSRect const & lim) const {
    int x11(po.x() - 1);
    int x12(po.x() + dp.x() + 1);
    int y11(po.y() - 1);
    int y12(po.y() + dp.y() + 1);
    int x21(lim.po.x());
    int x22(lim.po.x() + lim.dp.x());
    int y21(lim.po.y());
    int y22(lim.po.y() + lim.dp.y());
    int x1(x11 > x21 ? x11 : x21);
    int x2(x12 < x22 ? x12 : x22);
    int y1(y11 > y21 ? y11 : y21);
    int y2(y12 < y22 ? y12 : y22);
    return BSRect(BSPoint(x1, y1), BSPoint(x2-x1, y2-y1));
}

bool
BSRect::is_inside(BSPoint const & p) const {
    return (p.x() >= po.x() && p.x() < po.x() + dp.x() &&
            p.y() >= po.y() && p.y() < po.y() + dp.y());
}

// ----------------------

BSRect::iter::iter(BSRect const & p): rect(p), x(0), y(0) {}

BSPoint
BSRect::iter::operator*() const { return BSPoint(x, y) + rect.po; }

BSRect::iter &
BSRect::iter::operator++() {
    ++x;
    if (x >= rect.width()) {
        x = 0;
        ++y;
    }
    return *this;
}

bool
BSRect::iter::operator()() const { return (y < rect.height()); }

// ----------------------

BSRect::iter_inside::iter_inside(BSRect const & p, BSRect const & q):
    rect(p),
    inside_rect(q),
    x(0),
    y(0) {}

BSRect
BSRect::iter_inside::operator*() const {
    return BSRect(BSPoint(x, y) + rect.po, inside_rect.dp);
}

BSRect::iter_inside &
BSRect::iter_inside::operator++() {
    ++x;
    if (x > rect.width() - inside_rect.width()) {
        x = 0;
        ++y;
    }
    return *this;
}

bool
BSRect::iter_inside::operator()() const {
    return (y <= rect.height() - inside_rect.height());
}

void
BSRect::iter_inside::swap() {
    x = 0;
    y = 0;
    inside_rect.swap();
}

// --------------------------------------------------------

std::ostream & operator << (std::ostream& os, const BSRect & p) {
    std::cout << "BSRect(" << p.po << ", " << p.dp << ")"; // << std::endl;
    return os;
}

std::ostream & operator << (std::ostream& os, const BSPoint & p) {
    std::cout << "BSPoint(" << p.px << ", " << p.py << ")"; // << std::endl;
    return os;
}

