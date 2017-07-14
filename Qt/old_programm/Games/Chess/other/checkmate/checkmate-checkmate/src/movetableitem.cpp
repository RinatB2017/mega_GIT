/************************************************************************   
 * Copyright  2010  Miquel Canes Gonzalez  miquelcanes@gmail.com        *
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


#include "movetableitem.h"

MoveTableItem::MoveTableItem(bool white, ChessUtilities::Pieces type, const QString& text): QStandardItem(text)
{ 
	setIcon(ThemeProvider::self()->pieceToPixmap(type, 25));
	setCheckable(false);
	if(white){
		setBackground(QBrush(QColor(223, 223, 223)));
	}else{
		setBackground(QBrush(QColor(110, 110, 110)));
		setForeground(QBrush(Qt::white));
	}
}
