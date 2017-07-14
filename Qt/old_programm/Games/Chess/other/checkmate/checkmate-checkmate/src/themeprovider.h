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


#ifndef THEMEPROVIDER_H
#define THEMEPROVIDER_H

#include "piece.h"
#include "chessutilities.h"

#include <QSvgRenderer>
#include <QPixmapCache>

#include <QString>
#include <QGraphicsSvgItem>
#include <QPixmap>

class ThemeProvider
{
public:
    static ThemeProvider* self();
    bool loadCostumTheme(const QString& name);

    Piece* whitePawnSvg() const;
    Piece* whiteKnightSvg() const;
    Piece* whiteBishopSvg() const;
    Piece* whiteRockSvg() const;
    Piece* whiteQueenSvg() const;
    Piece* whiteKingSvg() const;

    Piece* blackPawnSvg() const;
    Piece* blackKnightSvg() const;
    Piece* blackBishopSvg() const;
    Piece* blackRockSvg() const;
    Piece* blackQueenSvg() const;
    Piece* blackKingSvg() const;

    Piece* pieceToSvg(ChessUtilities::Pieces type) const;

    QPixmap pieceToPixmap(ChessUtilities::Pieces type, int size) const;
    QPixmap boardToPixmap(int size) const;

    QGraphicsSvgItem* board() const;

private:
    ThemeProvider();

    QSvgRenderer* m_renderer;

    QString m_currentTheme;

    QPixmapCache* m_cache;

    QString elementToSvg(ChessUtilities::Pieces type) const;
};

#endif // THEMEPROVIDER_H
