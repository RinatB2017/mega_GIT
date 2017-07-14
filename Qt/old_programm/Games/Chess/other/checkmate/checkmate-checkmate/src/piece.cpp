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

#include "piece.h"

Piece::Piece(QSvgRenderer *renderer, const QString& svgItem, QGraphicsItem * parent):QGraphicsSvgItem(parent)
{
    setFlags(QGraphicsItem::ItemClipsToShape);
    setSharedRenderer(renderer);
    setElementId(svgItem);
}

Piece::~Piece()
{

}

void Piece::setSize ( qreal size )
{
    QRectF rect = boundingRect();
    scale(1/(rect.width()/size),1/(rect.height()/size));
}

