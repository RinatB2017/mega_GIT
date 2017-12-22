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

#include <QDebug>

#include "checkmate.h"
#include "advancegame.h"
#include "chessutilities.h"

#include <QDockWidget>
#include <QTableView>
#include <QTreeView>

#include <KApplication>
#include <KToolBar>
#include <KToggleAction>
#include <KStandardGameAction>
#include <KGameThemeSelector>
#include <KActionCollection>
#include <KAction>
#include <KLocale>
#include <KStatusBar>

Checkmate::Checkmate(QWidget* parent) : KXmlGuiWindow(parent)
{
	m_game = new GameInstance();
	m_board = m_game->getBoard();
	m_console = m_game->getGameConsole();
	m_movements = m_game->getTableModel();
	setCentralWidget(m_board);
	
	m_consoleDock = new QDockWidget(this);
	m_consoleDock->setWidget(m_console);
	m_consoleDock->setHidden(true);
	addDockWidget(Qt::BottomDockWidgetArea, m_consoleDock);
	
	m_movementsDock = new QDockWidget(this);
	QTableView* tablev = new QTableView(this);
	//QTreeView* tablev = new QTreeView(this);
	// tablev->setShowGrid(true);
	tablev->setModel(m_movements);
	m_movementsDock->setWidget(tablev);
	addDockWidget(Qt::RightDockWidgetArea, m_movementsDock);
	
	statusBar()->insertItem("       ", 0, 10);
	statusBar()->setItemAlignment(0, Qt::AlignLeft);
	statusBar()->insertItem("", 1);
	statusBar()->insertItem("", 2);
	statusBar()->insertItem("", 4);
	statusBar()->show();
	
	setupActions();
	setupGUI();
	setupLists();
	
	connect(m_game, SIGNAL(setColor(bool)), this, SLOT(changeColor(bool)));
	connect(m_game, SIGNAL(sendIllegalMove(QString,PieceInstance)), this, SLOT(showIllegalMove(QString,PieceInstance)));
	connect(m_game, SIGNAL(sendResult(QString,ChessUtilities::Result,QPoint)), this, SLOT(showResult(QString,ChessUtilities::Result,QPoint)));
	
	connect(KApplication::instance(), SIGNAL(lastWindowClosed()), this, SLOT(exit()));
}

Checkmate::~Checkmate()
{
	
}

void Checkmate::setupActions()
{
	KStandardGameAction::gameNew(this, SLOT(newGame()), actionCollection());
	KStandardGameAction::undo(m_game, SLOT(undo()), actionCollection());
	KGameDifficulty::init(this, this, SLOT(difficultyChanged(KGameDifficulty::standardLevel)));
	KGameDifficulty::setRestartOnChange(KGameDifficulty::NoRestartOnChange);
	KGameDifficulty::addStandardLevel(KGameDifficulty::Easy);
	KGameDifficulty::addStandardLevel(KGameDifficulty::Medium);
	KGameDifficulty::addStandardLevel(KGameDifficulty::Hard);
	KGameDifficulty::addStandardLevel(KGameDifficulty::ExtremelyHard);
	KGameDifficulty::setLevel(KGameDifficulty::Hard);
	KStandardGameAction::pause(this, SLOT(pause(bool)), actionCollection());
	KStandardGameAction::quit(this, SLOT(close()), actionCollection());
	//  KStandardGameAction::load(this, SLOT(load()), actionCollection());
	//  KStandardAction::preferences( this, SLOT( configureSettings()) , actionCollection() );
	//  KStandardGameAction::saveAs(this, SLOT(save()), actionCollection());
	
	KAction* advanceGame = new KAction(i18n("Configure New Game"), this);
	actionCollection()->addAction("advanceGame", advanceGame);
	connect(advanceGame, SIGNAL(triggered(bool)), this, SLOT(configureNewGame()));
	
	KToggleAction* movesTable = new KToggleAction(i18n("Show last moves table"), this);
	movesTable->setChecked(true);
	actionCollection()->addAction("movesTable", movesTable);
	connect(movesTable, SIGNAL(toggled(bool)), m_movementsDock, SLOT(setVisible(bool)));
	
	KToggleAction* console = new KToggleAction(i18n("Show console"), this);
	actionCollection()->addAction("console", console);
	connect(console, SIGNAL(toggled(bool)), m_consoleDock, SLOT(setVisible(bool)));
}

void Checkmate::setupLists()
{
	m_playerColor = new QActionGroup(this);
	m_playerColor->setExclusive(true);
	
	m_white = new KToggleAction(i18n("White"), this);
	m_white->setChecked(true);
	m_playerColor->addAction(m_white);
	connect(m_white, SIGNAL(toggled(bool)), this, SLOT(setWhite(bool)));
	
	m_black = new KToggleAction(i18n("Black"), this);
	m_playerColor->addAction(m_black);
	connect(m_black, SIGNAL(toggled(bool)), this, SLOT(setBlack(bool)));
	
	m_humans = new KToggleAction(i18n("Two players"), this);
	m_playerColor->addAction(m_humans);
	connect(m_humans, SIGNAL(toggled(bool)), this, SLOT(setBothHuman(bool)));
	
	plugActionList("colorGroup", m_playerColor->actions());
}

void Checkmate::difficultyChanged(KGameDifficulty::standardLevel difficulty)
{
	switch (difficulty) {
		case KGameDifficulty::Easy:
			m_game->changeDifficulty(ChessUtilities::EASY);
			break;
		case KGameDifficulty::Medium:
			m_game->changeDifficulty(ChessUtilities::MEDIUM);
			break;
		case KGameDifficulty::ExtremelyHard:
			m_game->changeDifficulty(ChessUtilities::EXTREME);
			break;
		case KGameDifficulty::Hard:
		default:
			m_game->changeDifficulty(ChessUtilities::HARD);
			break;
	}
}

void Checkmate::newGame()
{
	m_game->start();
}

void Checkmate::configureNewGame()
{
	AdvanceGame* dialog = new AdvanceGame();
	dialog->exec();
	qDebug() << "result " << dialog->result();
	if (dialog->result() == KDialog::Accepted) {
		qDebug() << "ok configure ";
		m_game->finished();
		ChessUtilities::Players player = dialog->player();
		switch(player){
			case ChessUtilities::WHITEHUMAN:
				m_white->setChecked(true);
				break;
			case ChessUtilities::BLACKHUMAN:
				m_black->setChecked(true);
				break;
			case ChessUtilities::BOTHHUMAN:
				m_humans->setChecked(true);
				break;
		}
		m_game->setStartFenPosition(dialog->initialFenPosition());
		m_game->start();
	}
}

void Checkmate::setWhite(bool check)
{
	if (check)
		m_game->setPlayers(ChessUtilities::WHITEHUMAN);
}

void Checkmate::setBlack(bool check)
{
	if (check)
		m_game->setPlayers(ChessUtilities::BLACKHUMAN);
}

void Checkmate::setBothHuman(bool check)
{
	if (check)
		m_game->setPlayers(ChessUtilities::BOTHHUMAN);
}

void Checkmate::exit()
{
	m_game->exit();
}

void Checkmate::showErrorMessage(const QString& message)
{
	statusBar()->changeItem(message, 1);
}

void Checkmate::changeColor(bool white)
{
	statusBar()->changeItem(white?i18n("White's turn"):i18n("Black's turn"), 0);
// 	statusBar()->showMessage(white?i18n("White's turn"):i18n("Black's turn"), 0);
}

void Checkmate::showIllegalMove(const QString& message, const PieceInstance& movement)
{
	statusBar()->changeItem(message, 1);
}

void Checkmate::showResult(const QString& message, ChessUtilities::Result res, const QPoint& kpos)
{
//	statusBar()->showMessage(message, 1);
	statusBar()->changeItem(message, 0);
}

void Checkmate::showInfoMessage(const QString& message)
{
	statusBar()->changeItem(message, 1);
}

void Checkmate::pause(bool checked)
{
	if(checked){
	bool res =	m_game->pause();
	if(!res)
		;//FIXME;
	}else
		m_game->resume();
}


