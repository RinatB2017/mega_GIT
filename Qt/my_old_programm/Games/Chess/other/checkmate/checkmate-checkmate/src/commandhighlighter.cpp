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


#include <QRegExp>
#include <QDebug>

#include "commandhighlighter.h"
#include "chessutilities.h"


CommandHighlighter::CommandHighlighter(const QStringList& reservedWords, QTextDocument* parent): QSyntaxHighlighter(parent)
{
	HighlightingRule rule;
	m_keywordFormat.setForeground(Qt::blue);
	m_keywordFormat.setFontWeight(QFont::Bold);

	foreach(const QString& word, reservedWords){
		rule.pattern = QRegExp("\\b" + word + "\\b");
		rule.format = m_keywordFormat;
		m_highlightingRules.append(rule);
		qDebug() << word;
	}

	m_numberFormat.setForeground(Qt::green);
	rule.pattern = QRegExp("\\b\\d+\\b");
	rule.format = m_numberFormat;
	m_highlightingRules.append(rule);

	m_coordenateFormat.setForeground(Qt::darkRed);
	m_coordenateFormat.setFontWeight(QFont::Bold);
	rule.pattern = QRegExp(ChessUtilities::REGEXPCOORDENATE);
	rule.format = m_coordenateFormat;
	m_highlightingRules.append(rule);

	m_sanFormat.setForeground(Qt::darkMagenta);
	m_sanFormat.setFontWeight(QFont::Bold);
	rule.pattern = QRegExp("\\b((N|B|R|Q|K|)([1-8]|)(x|)[a-h][1-8]((=|)(Q|B|N|R)|(#|\\+)|)|(O-O)|(O-O-O))\\b");
	rule.format = m_sanFormat;
	m_highlightingRules.append(rule);
}


void CommandHighlighter::highlightBlock(const QString& text)
{
	foreach (const HighlightingRule &rule, m_highlightingRules) {
		QRegExp expression(rule.pattern);
		int index = expression.indexIn(text);
		//qDebug << rule.pattern.;
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = expression.indexIn(text, index + length);
		}
	}
}

