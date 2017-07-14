/************************************************************************
 * Copyright  2010  Miquel Canes Gonzalez  miquelcanes@gmail.com        *
 *                                                                      *
 * This program is free software; you can redistribute it and/or        *
 * modify it under the terms of the GNU General Public License as       *
 * published by the Free Software Foundation; either version 2 of       *
 * the License, or (at your option) any later version.                  *
 *                                                                      *
 * This program is distributed in the hope that it will be useful,      *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.*
 ************************************************************************/

#ifndef MOVELIST_H
#define MOVELIST_H

#include <QList>

#include "movement.h"

class MoveList
{
	public:
		MoveList();
		~MoveList();

		void addMove(const Movement& movement);
		Movement getMove();
		void clean();

	private:
		QList<Movement> m_list;
};

#endif // MOVELIST_H
