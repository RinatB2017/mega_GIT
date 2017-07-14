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
#include <QLabel>
#include <QScrollBar>
#include <QPair>

#include <QLocale>

#include "gnuchessengine.h"
         
#include "chessutilities.h"
         
#include <signal.h>

const QString GnuChessEngine::REGEXPRESULT("0-1|1-0|1/2-1/2");

const QStringList GnuChessEngine::RESERVEDWORDS = QStringList() << "help" << "new" << "variant" << "quit" << "random" << "force" << "go"
 << "playother" << "white" << "black" << "st" << "sd" << "time" << "otim" /*<< "?"*/ << "ping" << "draw" << "result" << "setboard" << "edit"
 << "hint" << "bk" << "undo" << "remove" << "hard" << "easy" << "post" << "nopost" << "analyze" << "name" << "rating" << "ics" << "computer"
 << "pause" << "resume";

GnuChessEngine::GnuChessEngine()
{
	m_manual = false;
	m_engine = new QProcess();
	m_engine->setOutputChannelMode(QProcess::SeparateChannels);
	m_engine->setNextOpenMode(QIODevice::ReadWrite);
	
	whiteMoveRegEx = QRegExp("\\. " + ChessUtilities::REGEXPCOORDENATE);
	blackMoveRegEx = QRegExp("\\. \\.\\.\\. " + ChessUtilities::REGEXPCOORDENATE);
	illegalMoveRegEx = QRegExp("Illegal move(?::| \\(([a-zA-Z0-9\\s]+)\\) :) (\\S+)");
	resultOutputRegEx = QRegExp("(" + REGEXPRESULT + ")" + " \\{([a-zA-Z0-9\\s]+)\\}");
	coordenateRegEx = QRegExp(ChessUtilities::REGEXPCOORDENATE);
	
	QObject::connect(m_engine, SIGNAL(readyReadStandardOutput()), this, SLOT(reciveInput()));
	QObject::connect(m_engine, SIGNAL(readyReadStandardError()), this, SLOT(reciveInput()));
}

void GnuChessEngine::start()
{
	qDebug() << "starting chess";
	m_engine->setProgram("gnuchess", QStringList("-x"));
	m_engine->start();
	m_pid = m_engine->pid();
	m_engine->write("xboard\n");
	m_engine->write("protover 2\n");
}


void GnuChessEngine::sendMove(const QString& movement)
{
	sendCommand(movement.toUtf8());
}

void GnuChessEngine::reciveInput()
{
	//Parse all output form stdout and stderr and send signals
	QString output = m_engine->readAllStandardOutput();
	qDebug() << "recieveInput : " << output;
	emit sendOutput(output);
	QStringList lines = output.split("\n");
	for(int i = 0; i < lines.size(); i++)
	{
	QString line = lines.at(i);
	
        if(illegalMoveRegEx.indexIn(line) >= 0)
        {
		QString pos;
		if(coordenateRegEx.exactMatch(illegalMoveRegEx.cap(2)))
		{
			pos =illegalMoveRegEx.cap(2);
		}
		emit illegalMove(illegalMoveRegEx.cap(1), pos);
		qDebug() << "Illegal Move " << illegalMoveRegEx.cap(1) << "" << illegalMoveRegEx.cap(2);
        }
        else if(resultOutputRegEx.exactMatch(line))
        {
		qDebug() << "result";
		QString sresult = resultOutputRegEx.cap(1);
		ChessUtilities::Result winner;
		QString messageMan;
		if(sresult == "0-1")
		{
			winner = ChessUtilities::BLACKSWIN;
			messageMan = i18n("Black player wins");
		}
		else if(sresult == "1-0")
		{
			winner = ChessUtilities::WHITESWIN;
			messageMan = i18n("White player wins");
		}
		else if(sresult == "1/2-1/2")
			winner = ChessUtilities::DRAW;
		else
			Q_ASSERT(false);                //REMOVE: only test propouse
			QString message = resultOutputRegEx.cap(2);
			if(m_manual)
				message = messageMan;
			emit result(message, winner);
        }
        else if(whiteMoveRegEx.indexIn(line) >= 0)
        {
		emit confirmedMove(whiteMoveRegEx.cap(1) + whiteMoveRegEx.cap(2));
        }
        else if(blackMoveRegEx.indexIn(line) >= 0)
        {
		emit confirmedMove(blackMoveRegEx.cap(1) + blackMoveRegEx.cap(2));
	}
	}
}

QString GnuChessEngine::getStartPosition() const
{
	//Gnuchess only supports classic chess game
	return QString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}


void GnuChessEngine::sendCommand(const QString& command)
{
	qDebug() << "sendCommand:" << command;
	m_engine->write(command.toUtf8() + "\n");
}


QStringList GnuChessEngine::getReservedWords() const
{
	return RESERVEDWORDS;
}

void GnuChessEngine::playActualColor(bool white)
{
	sendCommand("force");
	m_manual = false;
	if(white)
		sendCommand("white");
	else
		sendCommand("black");
	sendCommand("go");
}

void GnuChessEngine::playOtherColor(bool white)
{
	//FIXME: Support provoter v1 with white black commands
	sendCommand("force");
	m_manual = false;
	//TODO:
	bool playother = false; 
	if(playother)
	sendCommand("playother");
	else{
	if(white)
		sendCommand("black");
	else
		sendCommand("white");
}
}

void GnuChessEngine::undo(bool white, bool enginePlay)
{
	//  if(!enginePlay && !m_manual)
	//    sigint();
	sendCommand("force");
	sendCommand("undo");
	if(!m_manual){
		// if(enginePlay){
		//  playActualColor(!white);
		//}else{
		sendCommand("undo");
		playOtherColor(!white);
		//}
	}
}

void GnuChessEngine::manual()
{
	sendCommand("force");
	m_manual = true;
}

void GnuChessEngine::exit()
{
	sendCommand("exit");
	m_engine->terminate();
}

int GnuChessEngine::sigint()
{
	return kill(m_pid, SIGINT);
}

void GnuChessEngine::difficulty(ChessUtilities::Difficulty difficulty)
{
	switch(difficulty){
		case ChessUtilities::EASY:
		case ChessUtilities::MEDIUM:
			sendCommand("easy");
			break;
		case ChessUtilities::HARD:
		case ChessUtilities::EXTREME:
			sendCommand("hard");
			break;
	}
}

void GnuChessEngine::setFENPosition(const QString& fen)
{
	sendCommand("setboard " + fen);
}


void GnuChessEngine::pause()
{
	sendCommand("force");
}

void GnuChessEngine::resume(bool white, bool engine)
{
	if(white)
		sendCommand("white");
	else
		sendCommand("black");
	
	if(engine)
		sendCommand("go");
}

void GnuChessEngine::hint()
{
	sendCommand("hint");
}

bool GnuChessEngine::san()
{
	//TODO:
	return false;
}
