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

#ifndef BOARD_H
#define BOARD_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSvgItem>
#include <QPoint>
#include <QChar>
#include <QList>

#include "piece.h"
#include "chessutilities.h"
#include "pieceinstance.h"

class Board : public QGraphicsView
{
	Q_OBJECT
	public:
		Board();
		void initPosition();
		void setVisibleCoord(const bool& visible);
		void setInitialPiece(ChessUtilities::Pieces pieceType, const QPoint& pos);
		void clean();
		void enabled(bool enabled);
		
	protected:
		void mousePressEvent(QMouseEvent *event);
		void resizeEvent(QResizeEvent *event); 

	public slots:
		void setMove(const PieceInstance& movement);
		void removePiece(const QPoint& pos);
		void illegalMove(const QString& message, const PieceInstance& movement);
		void showErrorMessage(const QString& message);
		void showInfoMessage(const QString& message);
		void showIllegalMessage(const QString& message);
		void changeColor(bool white);
		void promotion(const PieceInstance& movement);
		void showResult(const QString& message, ChessUtilities::Result res, const QPoint& kpos);
		
	signals:
		void newMove(const PieceInstance& movement);

	private:
		static const int PIECEKEY;
		void centerView();
		Piece* newPiece(ChessUtilities::Pieces pieceType);
		void drawCoord();
		QGraphicsItem* getPiece(const QPointF& from);
		QPoint realPosToMatrix(const QPointF& realPos);
		void removeMarkRect(QGraphicsRectItem* rect);
		void setColors();
		QPen m_qpenSelected;
		QPen m_qpenlastMove;
		QPen m_qpenIllegalMove;
		QPen m_qpenwinner;
		QBrush m_qbrushwinner;
		QBrush m_qbrushloser;
		QBrush m_qbrushtransparent;
		QGraphicsScene* m_scene;
		bool m_showCoord;
		bool m_pieceIsSelected;
		QPoint m_selectedPiece;
		QGraphicsRectItem* m_selectedRect;
		QGraphicsRectItem* m_whiteMovedFromRect;
		QGraphicsRectItem* m_whiteMovedToRect;
		QGraphicsRectItem* m_blackMovedFromRect;
		QGraphicsRectItem* m_blackMovedToRect;
		QGraphicsRectItem* m_illegalFromRect;
		QGraphicsRectItem* m_illegalToRect;
		QGraphicsRectItem* m_loserRect;
		bool m_whitePlayer;
		bool m_active;
		
		QGraphicsSimpleTextItem** coord;
};

#endif // BOARD_H
