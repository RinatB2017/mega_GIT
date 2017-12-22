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

#include <QStringList>

#include "chessutilities.h"

const QString ChessUtilities::letterBoardMapToMatrix = "abcdefgh";
const QString ChessUtilities::REGEXPCOORDENATE = "\\b([a-h][1-8][a-h][1-8])(q|r|b|n|)\\b";

PieceInstance ChessUtilities::LongAlgebraicToMatrix(const QString& algebraic)
{
	PieceInstance pinstance;
	QString split;
	
	if(algebraic.contains("x")){
		split = "x";
	}else{
		split = "-";
	}
	QStringList notation = algebraic.split("-");
//	fromTo.first = AlgebraicUnitToMatrix(notation.at(0));
//	fromTo.second = AlgebraicUnitToMatrix(notation.at(1));
	return pinstance;
}

QPoint ChessUtilities::AlgebraicUnitToMatrix(const QString& algebraicUnit)
{
	QPoint matrixPos;
	matrixPos.setX(letterBoardMapToMatrix.indexOf(algebraicUnit.at(0)) + 1);
	matrixPos.setY(9 - algebraicUnit.at(1).digitValue() );
	return matrixPos;
}


QString ChessUtilities::MatrixToLongAlgebraic(const QPoint& from, const QPoint& to, const bool& capture)
{
	//TODO:
	return QString("o");
}

PieceInstance ChessUtilities::CoordenateToMatrix(const QString& coordenate)
{
	PieceInstance pinstance;
	pinstance.from = AlgebraicUnitToMatrix(coordenate.left(2));
	pinstance.to = AlgebraicUnitToMatrix(coordenate.mid(2,2));
	if(coordenate.length() == 5)
		pinstance.promotion = coordenate.at(4);
	return pinstance;
}

QString ChessUtilities::MatrixToCoordenate(const PieceInstance& movement)
{
	return MatrixUnitToAlgebraic(movement.from) + MatrixUnitToAlgebraic(movement.to) + movement.promotion;
}


QString ChessUtilities::MatrixUnitToAlgebraic(const QPoint& pos)
{
	QString algebraic = letterBoardMapToMatrix.at(pos.x() -1);
	algebraic.append(QString().setNum(9 - pos.y()));
	return algebraic;
}


ChessUtilities::Pieces ChessUtilities::FenToPieces ( const QChar& fen )
{
	Pieces piece = NOPIECE;
	switch(fen.toAscii()){
	case 'p':
		piece = BPAWN;
		break;
	case 'P':
		piece = WPAWN;
		break;
	case 'n':
		piece = BKNIGHT;
		break;
	case 'N':
		piece = WKNIGHT;
		break;
	case 'b':
		piece = BBISHOP;
		break;
	case 'B':
		piece = WBISHOP;
		break;
	case 'r':
		piece = BROCK;
		break;
	case 'R':
		piece = WROCK;
		break;
	case 'q':
		piece = BQUEEN;
		break;
	case 'Q':
		piece = WQUEEN;
		break;
	case 'k':
		piece = BKING;
		break;
	case 'K':
		piece = WKING;
		break;
	}
	return piece;
}


bool ChessUtilities::isWhitePiece(const ChessUtilities::Pieces& piece)
{
	if(piece == WPAWN || piece == WKING || piece == WQUEEN || piece == WROCK || piece == WKNIGHT || piece == WBISHOP)
		return true;
	else
		return false;
}


QChar ChessUtilities::PiecesToFen(const ChessUtilities::Pieces& piece)
{
	QChar c;
	switch(piece){
	case ChessUtilities::WPAWN:
		c = 'P';
		break;
	case ChessUtilities::BPAWN:
		c = 'p';
		break;
	case ChessUtilities::WROCK:
		c = 'R';
		break;
	case ChessUtilities::BROCK:
		c = 'r';
		break;
	case ChessUtilities::WKNIGHT:
		c = 'N';
		break;
	case ChessUtilities::BKNIGHT:
		c = 'n';
		break;
	case ChessUtilities::WBISHOP:
		c = 'B';
		break;
	case ChessUtilities::BBISHOP:
		c = 'b';
		break;
	case ChessUtilities::WQUEEN:
		c = 'Q';
		break;
	case ChessUtilities::BQUEEN:
		c = 'q';
		break;
	case ChessUtilities::WKING:
		c = 'K';
		break;
	case ChessUtilities::BKING:
		c = 'k';
		break;
	case ChessUtilities::NOPIECE:
		c = ' ';
		break;
	}
	return c;
}

