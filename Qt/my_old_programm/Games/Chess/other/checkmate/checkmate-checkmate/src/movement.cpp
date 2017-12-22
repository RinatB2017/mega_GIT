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

#include "movement.h"

Movement::Movement(const PieceInstance& movement)
{
	main = movement;
	aux = PieceInstance();
	m_capturedPiece = ChessUtilities::NOPIECE;
}

void Movement::setAuxMove(const PieceInstance& movement)
{
	aux = movement;
}

void Movement::setCapturedPiece(ChessUtilities::Pieces captured)
{
	m_capturedPiece = captured;
}

PieceInstance Movement::move() const
{
	return main;
}

PieceInstance Movement::auxMove() const
{
	return aux;
}

ChessUtilities::Pieces Movement::capturedPiece()
{
	return m_capturedPiece;
}

