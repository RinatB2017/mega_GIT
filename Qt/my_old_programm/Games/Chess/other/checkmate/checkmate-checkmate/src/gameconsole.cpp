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

#include "gameconsole.h"

#include <QBoxLayout>
#include <QPushButton>

GameConsole::GameConsole(const QStringList& reservedWords, QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
	m_commandText = new CommandEdit();
	m_highlighter = new CommandHighlighter(reservedWords, m_commandText->document());
	m_oldTexts = new KRichTextEdit;
	m_oldTexts->setMinimumHeight(50);
	QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom);
	layout->addWidget(m_oldTexts);
	layout->addWidget(m_commandText);
	QPushButton* send = new QPushButton();
	send->setText("Send"); //FIXME: i18n
	connect(send, SIGNAL(clicked(bool)), this, SLOT(slotSendCommand()));
	layout->addWidget(send);
	this->setLayout(layout);
}


GameConsole::~GameConsole()
{
	
}

void GameConsole::clean()
{
	m_oldTexts->clear();
}

void GameConsole::engineOutput(const QString& output)
{
	m_oldTexts->append(output);
}


void GameConsole::slotSendCommand()
{
	if(!m_commandText->toPlainText().isEmpty()){
		QString command = m_commandText->toPlainText();
		m_oldTexts->append(command);
		emit sendCommand(command);
		m_commandText->clear();
	}
}
