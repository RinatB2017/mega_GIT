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

#ifndef ABSTRACTENGINE_H
#define ABSTRACTENGINE_H

#include <QObject>
#include <QPoint>
#include <QStringList>
#include <QString>
#include <QChar>

#include "chessutilities.h"

class AbstractEngine : public QObject
{   
	Q_OBJECT
    public:
		virtual ~AbstractEngine() {}
		//X-FEN position
		virtual void start() = 0;
		virtual QString getStartPosition() const =  0;
		virtual QStringList getReservedWords() const = 0;
		virtual void playActualColor(bool white) = 0;
		virtual void playOtherColor(bool white) = 0;
		virtual void manual() = 0;
		virtual void undo(bool white, bool enginePlay) = 0;
		virtual void exit() = 0;
		virtual void setFENPosition(const QString& fen) = 0;
		virtual void pause() = 0;
		virtual void resume(bool white, bool engine) = 0;
		virtual void hint() = 0;
		virtual bool san() = 0;
		
    public slots:
		virtual void sendMove(const QString& movement) = 0;
		virtual void sendCommand(const QString& command) = 0;
		virtual void difficulty(ChessUtilities::Difficulty difficulty) = 0;
	
    signals:
		virtual void confirmedMove(const QString& movement) = 0;
		virtual void illegalMove(const QString& message, const QString& movement) = 0;
		virtual void sendOutput(const QString& output) = 0;
		virtual void result(const QString& comment, ChessUtilities::Result res) = 0;
		virtual void sendHint(const QPoint& pos) = 0;
};

#endif // ABSTRACTENGINE_H
