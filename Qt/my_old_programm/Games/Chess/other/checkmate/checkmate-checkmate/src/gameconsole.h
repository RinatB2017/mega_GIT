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

#ifndef GAMECONSOLE_H
#define GAMECONSOLE_H

#include <QWidget>
#include <krichtextedit.h>
#include <QBoxLayout>

#include "commandedit.h"

class GameConsole : public QWidget
{
	Q_OBJECT
	public:
		GameConsole(const QStringList& reservedWords, QWidget* parent = 0, Qt::WindowFlags f = 0);
		~GameConsole();
		void clean();

	public slots:
		void engineOutput(const QString& output);
		void slotSendCommand();

	signals:
		void sendCommand(const QString& command);
	
	private:
		KRichTextEdit* m_oldTexts;
		CommandHighlighter* m_highlighter;
		CommandEdit* m_commandText;
		QBoxLayout* m_layout;

};

#endif // GAMECONSOLE_H
