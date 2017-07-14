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

#ifndef kchess_kchess_H
#define kchess_kches_H

#include "gameconsole.h"
#include "board.h"
#include "gameinstance.h"
#include "movetablemodel.h"
#include "pieceinstance.h"
         
#include <QDockWidget>
#include <QActionGroup>
#include <QPoint>

#include <QXmlGuiWindow>
#include <QActionMenu>
#include <QGameDifficulty>
#include <QToggleAction>

class Checkmate : public KXmlGuiWindow
{
	Q_OBJECT
	public:
		Checkmate(QWidget* parent=0);
		virtual ~Checkmate();
		
	public slots:
		void newGame();
		void setWhite(bool check);
		void setBlack(bool check);
		void setBothHuman(bool check);
		void exit();
		void difficultyChanged(KGameDifficulty::standardLevel difficulty);
		void configureNewGame();
		void pause(bool checked);
		
		void showErrorMessage(const QString& message);
		void showInfoMessage(const QString& message);
		void showIllegalMove(const QString& message, const PieceInstance& movement);
		void changeColor(bool white);
		void showResult(const QString& message, ChessUtilities::Result res, const QPoint& kpos);
		
		
	private:
		void setupActions();
		void setupLists();
		GameInstance* m_game;
		Board* m_board;
		GameConsole* m_console;
		QAbstractItemModel* m_movements;
		
		QDockWidget* m_consoleDock;
		QDockWidget* m_movementsDock;
		
		KToggleAction* m_white;
		KToggleAction* m_black;
		KToggleAction* m_humans;
		
		QActionGroup* m_playerColor;
		
};

#endif
