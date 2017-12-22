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


#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <QPoint>
#include <QPair>

#include "chessutilities.h"
#include "pieceinstance.h"
#include "piece.h"
		 
class Movement
{
	public:
		Movement(const PieceInstance& movement);
		void setAuxMove(const PieceInstance& movement);
		void setCapturedPiece(ChessUtilities::Pieces captured = ChessUtilities::NOPIECE);
		PieceInstance move() const;
		PieceInstance auxMove() const;
		ChessUtilities::Pieces capturedPiece();

	private:
		//Movement
		PieceInstance main;
		//Captured Piece
		ChessUtilities::Pieces m_capturedPiece;
		//Auxiliar Move for captured , en passant, castling
		PieceInstance aux;
};

#endif // MOVEMENT_H
