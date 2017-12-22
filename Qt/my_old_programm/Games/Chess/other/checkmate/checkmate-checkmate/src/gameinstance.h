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

#ifndef GAMEINSTANCE_H
#define GAMEINSTANCE_H

#include "board.h"
#include "abstractengine.h"
#include "gameconsole.h"
#include "movetablemodel.h"
#include "movelist.h"
#include "movement.h"
#include "pieceinstance.h"
         
#include <QObject>
#include <QStandardItemModel>

class GameInstance: public QObject
{
	Q_OBJECT
	public:
		GameInstance();
		~GameInstance();
		void newGame();
		void start();
		void exit();
		void finished();
		
		Board* getBoard();
		GameConsole* getGameConsole();
		QAbstractItemModel* getTableModel();
     
	public slots:
		/**
		 * Board send a new move to the game instance. 
		 * If its correct (basic checks). It's send to the engine.
		 */
		void byNewMove(const PieceInstance& movement);
		
		/**
		 * The engine sets a new movement.
		 * Game instance translate it and send to the board; 
		 */
		void bySetMove(const QString& movement);
		void bySendCommand(const QString& command);
		void engineOutput(const QString& output);
		void setPlayers(ChessUtilities::Players type);
		void setWhite();
		void setBlack();
		void setBothHumans();
		void undo();
		void result(const QString& message, ChessUtilities::Result res);
		void changeDifficulty(ChessUtilities::Difficulty difficulty);
		void setStartFenPosition(const QString& fen);
		void reciveIllegalMove(const QString& message, const QString& movement);
		bool pause();
		void resume();
		
	signals:
//		void newMove(const QPoint& from, const QPoint& to, const QString& promotion);
//		void setMove(const QPoint& from, const QPoint& to, const QString& promotion);
		void sendCommand(const QString& command);
		void sendEngineOutput(const QString& output);
		void sendErrorMessage(const QString& message);
		void sendInfoMessage(const QString& message);
		void sendIllegalMove(const QString& message, const PieceInstance& movement);
		void sendResult(const QString& message, ChessUtilities::Result res, const QPoint& kpos);
		void setColor(bool white);
//		void askPromotion(const QPoint& from, const QPoint& to);
//		void removePiece(const QPoint& pos);
		
	private:
		void undoMovement();
		QList<QPoint>* findPieces(ChessUtilities::Pieces type);
		void clean();
		bool initValues();
		bool getFENPosition(const QString& fen);
		bool setFENPosition(const QString& fen);
		ChessUtilities::Pieces getInstancePiece(const QPoint& pos);
		void setInstancePiece(const QPoint& pos, ChessUtilities::Pieces piece);
		PieceInstance engineToMatrix(const QString& movement) const;
		QString matrixToEngine(PieceInstance movement) const;
		void printBoard();
		Board* m_board;
		GameConsole* m_gameConsole;
		MoveTableModel* m_movetable;
		AbstractEngine* m_engine;
		ChessUtilities::Pieces** m_instanceBoard;
		bool m_active;
		bool m_started;
		bool m_whiteBlackTurn;
		bool m_whiteQCastle;
		bool m_whiteKCastle;
		bool m_blackQCastle;
		bool m_blackKCastle;
		QPoint m_enPassant;
		int m_fiftyMoveRuleCount;
		int m_movementsCount;
		bool m_whitePlayer;
		bool m_blackPlayer;
		ChessUtilities::Difficulty m_difficulty;
		MoveList* m_moveList;
		QString m_startFenPosition;
		bool m_finished;
};

#endif // GAMEINSTANCE_H
