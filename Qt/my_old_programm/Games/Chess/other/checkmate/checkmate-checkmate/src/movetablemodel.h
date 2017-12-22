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


#ifndef MOVETABLEMODEL_H
#define MOVETABLEMODEL_H

#include "movetableitem.h"

#include <QStandardItemModel>
#include <QList>
#include <QString>

class MoveTableModel : public QStandardItemModel
{
	Q_OBJECT
	public:
		MoveTableModel(QObject* parent = 0);
		void clean();
	// int rowCount(const QModelIndex &parent = QModelIndex()) const;
	// int columnCount(const QModelIndex &parent = QModelIndex()) const;
		
	// QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	// QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
		
	// public slots:
		void addMove(const QString& move, ChessUtilities::Pieces type);
		void removeLastMove();
		
	private:
		int m_count;
		bool m_white;
	//  QList<MoveTableItem> m_move;
};

#endif // MOVETABLEMODEL_H
