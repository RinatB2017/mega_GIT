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
#include "flags.h"

BSFlags::BSFlags(): flags(0) {}

void BSFlags::clean()  { flags = 0; }
void BSFlags::occupy() { flags |= 1; }
void BSFlags::fire()   { flags |= 2; }
void BSFlags::mark()   { flags |= 4; }

void BSFlags::unfire() { flags &= 1|4; }
void BSFlags::unmark() { flags &= 1|2; }

bool BSFlags::occupied() const { return (flags & 1) != 0; }
bool BSFlags::fired()    const { return (flags & 2) != 0; }
bool BSFlags::marked()   const { return (flags & 4) != 0; }
