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


#ifndef COMMANDHIGHLIGHTER_H
#define COMMANDHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QStringList>
#include <QTextDocument>

class CommandHighlighter : public QSyntaxHighlighter
{
	Q_OBJECT
	public:
		CommandHighlighter(const QStringList& reservedWords, QTextDocument* parent);  

	protected:
		void highlightBlock(const QString& text);

	private:
		struct HighlightingRule{
			QRegExp pattern;
			QTextCharFormat format;
		};
		
		QVector<HighlightingRule> m_highlightingRules;
		
		QTextCharFormat m_keywordFormat;
		QTextCharFormat m_numberFormat;
		QTextCharFormat m_coordenateFormat; //e2e4
		QTextCharFormat m_sanFormat;        //e4, exd5, O-O
};

#endif // COMMANDHIGHLIGHTER_H
