/************************************************************************   
 * Copyright  2009  Miquel Canes Gonzalez  miquelcanes@gmail.com        *
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

#ifndef CHESSUTILITIES_H
#define CHESSUTILITIES_H

#include <QPair>
#include <QPoint>
#include <QString>
#include <QChar>

#include "pieceinstance.h"

class ChessUtilities
{
public:
    enum Pieces { NOPIECE, WPAWN, WROCK, WKNIGHT, WBISHOP, WKING, WQUEEN, BPAWN, BROCK, BKNIGHT, BBISHOP, BKING, BQUEEN };
    enum Result {WHITESWIN, BLACKSWIN, DRAW};
    enum Difficulty {EASY, MEDIUM, HARD, EXTREME};
    enum Players {WHITEHUMAN, BLACKHUMAN, BOTHHUMAN, BOTHENGINES};
    static const QString REGEXPCOORDENATE;
    static PieceInstance LongAlgebraicToMatrix(const QString& algebraic);
    static QString MatrixToLongAlgebraic(const QPoint& from, const QPoint& to, const bool& capture);
    static PieceInstance CoordenateToMatrix(const QString& coordenate);
    static QString MatrixToCoordenate(const PieceInstance& movement);
    static Pieces FenToPieces(const QChar& fen);
    static QChar PiecesToFen(const Pieces& piece);
    static QPoint AlgebraicUnitToMatrix(const QString& algebraicUnit);
    static bool isWhitePiece(const Pieces& piece);
    static QString MatrixUnitToAlgebraic(const QPoint& pos);
private:
    static const QString letterBoardMapToMatrix;
};

#endif // CHESSUTILITIES_H
