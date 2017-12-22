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
#ifndef BS_LOGIC_MAP_H
#define BS_LOGIC_MAP_H

#include "geometry.h"

template<class T>
class BSMap {
private:
    BSRect rect;
    T * field_map;
public:
    BSMap(BSRect const & r);
    BSMap(const BSMap &); /// IMPLEMENT IT!!
    ~BSMap();
    T * const operator[](BSPoint const & p) const;
    BSRect get_rect() const;
    BSMap & operator=(const BSMap &); /// IMPLEMENT IT!!
//    BSMap * operator&(); /// IMPLEMENT IT!?
//    BSMap const * operator&() const; /// IMPLEMENT IT!?
};

// -------------------------------------

template<class T>
BSMap<T>::BSMap(BSRect const & r):
        rect(r),
        field_map(new T[r.square()]) {}

template<class T>
BSMap<T>::~BSMap() { delete[] field_map; }

template<class T>
T * const
BSMap<T>::operator[](BSPoint const & p) const {
    return field_map + rect.idx(p);
}

template<class T>
BSRect
BSMap<T>::get_rect() const { return rect; }

#endif

