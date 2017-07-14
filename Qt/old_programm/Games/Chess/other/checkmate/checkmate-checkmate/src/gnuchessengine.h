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

#ifndef GNUCHESSENGINE_H
#define GNUCHESSENGINE_H

#include <QProcess>

#include <QRegExp>
#include "abstractengine.h"
#include "chessutilities.h"

class GnuChessEngine : public AbstractEngine
{   
    Q_OBJECT

public:
    GnuChessEngine();
    void start();
    QString getStartPosition() const;
    QStringList getReservedWords() const;
    void playActualColor(bool white);
    void playOtherColor(bool white);
    void undo(bool white, bool enginePlay);
    void setFENPosition(const QString& fen);
    void manual();
    void exit();
    void pause();
    void resume(bool white, bool engine);
    void hint();
    bool san();

public slots:
    void sendMove(const QString& movement);
    void sendCommand(const QString& command);
    void reciveInput();
    void difficulty(ChessUtilities::Difficulty difficulty);

signals:
    void confirmedMove(const QString& movement);
    void illegalMove(const QString& message, const QString& movement);
    void sendOutput(const QString& output);
    void result(const QString& comment, ChessUtilities::Result res);
    void sendHint(const QPoint& pos);

private:
    int sigint();
    QProcess* m_engine;
    bool m_manual;
    int m_pid;
    static const QString REGEXPRESULT;
    static const QStringList RESERVEDWORDS;
    QRegExp whiteMoveRegEx;
    QRegExp blackMoveRegEx;
    QRegExp illegalMoveRegEx;
    QRegExp resultOutputRegEx;
    QRegExp coordenateRegEx;
};

#endif // GNUCHESSENGINE_H
