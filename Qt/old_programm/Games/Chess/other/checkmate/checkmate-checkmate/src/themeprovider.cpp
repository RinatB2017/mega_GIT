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

#include "themeprovider.h"

#include <QDebug>
#include <QPainter>

#include <KGameTheme>

#include "settings.h"

ThemeProvider::ThemeProvider()
{
	m_renderer = new KSvgRenderer();
	m_cache = new KPixmapCache("checkmate-cache");
	if(!loadCostumTheme(Settings::theme()))
		qDebug() << "Fail to load default theme";
}

ThemeProvider* ThemeProvider::self()
{
	static ThemeProvider instance;
	return &instance;
}

bool ThemeProvider::loadCostumTheme(const QString& name)
{
	if(!m_currentTheme.isEmpty() && m_currentTheme == name)
		return true;
	
	m_currentTheme = name;
	
	KGameTheme theme;
	if(name.isEmpty() || !theme.load(name)){
		if(!theme.loadDefault())
			return false;
		m_currentTheme = "default";
	}
	
	if(!m_renderer->load(theme.graphics()))
		return false;
	
	return true;
}

QString ThemeProvider::elementToSvg(ChessUtilities::Pieces type) const
{
	switch(type){
		case ChessUtilities::WPAWN:
			return "wp";
		case ChessUtilities::BPAWN:
			return "bp";
		case ChessUtilities::WROCK:
			return "wr";
		case ChessUtilities::BROCK:
			return "br";
		case ChessUtilities::WKNIGHT:
			return "wn";
		case ChessUtilities::BKNIGHT:
			return "bn";
		case ChessUtilities::WBISHOP:
			return "wb";
		case ChessUtilities::BBISHOP:
			return "bb";
		case ChessUtilities::WQUEEN:
			return "wq";
		case ChessUtilities::BQUEEN:
			return "bq";
		case ChessUtilities::WKING:
			return "wk";
		case ChessUtilities::BKING:
			return "bk";
		case ChessUtilities::NOPIECE:
		default:
			return QString();
	}
}

Piece* ThemeProvider::pieceToSvg(ChessUtilities::Pieces type) const
{
	Piece* piece = new Piece(m_renderer, elementToSvg(type));
	return piece;
}


QPixmap ThemeProvider::pieceToPixmap(ChessUtilities::Pieces type, int size) const
{
	QPainter p;  
	QPixmap pix;
	QString elementId = elementToSvg(type);
	QString cacheName = elementId+QString::number(size);
	if (!m_cache->find(cacheName, pix)){
		pix = QPixmap(size, size);
		pix.fill( Qt::transparent);
		p.begin( &pix );
		m_renderer->render( &p, elementId );
		p.end();
		m_cache->insert(cacheName, pix);
	}
	return pix;
}

QPixmap ThemeProvider::boardToPixmap(int size) const
{
	QPainter p;  
	QPixmap pix;
	QString cacheName = "board"+QString::number(size);
	if (!m_cache->find(cacheName, pix)){
		pix = QPixmap(size, size);
		pix.fill( Qt::transparent);
		p.begin( &pix );
		m_renderer->render( &p, "board" );
		p.end();
		m_cache->insert(cacheName, pix);
	}
	return pix;
}

Piece* ThemeProvider::whitePawnSvg() const
{
	Piece* piece = pieceToSvg(ChessUtilities::WPAWN);
	piece->setSize(100);
	return piece;
}


Piece* ThemeProvider::blackPawnSvg() const
{
	Piece* piece = pieceToSvg(ChessUtilities::BPAWN);
	piece->setSize(100);
	return piece;
}


Piece* ThemeProvider::whiteKnightSvg() const
{
	Piece* piece = pieceToSvg(ChessUtilities::WKNIGHT);
	piece->setSize(100);
	return piece;
}


Piece* ThemeProvider::blackKnightSvg() const
{
	Piece* piece = pieceToSvg(ChessUtilities::BKNIGHT);
	piece->setSize(100);
	return piece;
}


Piece* ThemeProvider::whiteBishopSvg() const
{
	Piece* piece = pieceToSvg(ChessUtilities::WBISHOP);
	piece->setSize(100);
	return piece;
}


Piece* ThemeProvider::blackBishopSvg() const
{
	Piece* piece = pieceToSvg(ChessUtilities::BBISHOP);
	piece->setSize(100);
	return piece;
}


Piece* ThemeProvider::whiteRockSvg() const
{
	Piece* piece = pieceToSvg(ChessUtilities::WROCK);
	piece->setSize(100);
	return piece;
}


Piece* ThemeProvider::blackRockSvg() const
{
	Piece* piece = pieceToSvg(ChessUtilities::BROCK);
	piece->setSize(100);
	return piece;
}


Piece* ThemeProvider::whiteQueenSvg() const
{
	Piece* piece = pieceToSvg(ChessUtilities::BKING);
	piece->setSize(100);
	return piece;
}


Piece* ThemeProvider::blackQueenSvg() const
{
	Piece* piece = pieceToSvg(ChessUtilities::BQUEEN);
	piece->setSize(100);
	return piece;
}


Piece* ThemeProvider::whiteKingSvg() const
{
	Piece* piece = pieceToSvg(ChessUtilities::WKING);
	piece->setSize(100);
	return piece;
}


Piece* ThemeProvider::blackKingSvg() const
{
	Piece* piece = pieceToSvg(ChessUtilities::BKING);
	piece->setSize(100);
	return piece;
}


QGraphicsSvgItem* ThemeProvider::board() const
{
	QGraphicsSvgItem* boardsvg = new QGraphicsSvgItem();
	boardsvg->setSharedRenderer(m_renderer);
	boardsvg->setElementId("board");
	return boardsvg;
}
