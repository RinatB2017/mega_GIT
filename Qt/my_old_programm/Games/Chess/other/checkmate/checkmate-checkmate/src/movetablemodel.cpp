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


#include "movetablemodel.h"
         
#include <QVariant>
#include <KLocale>
#include <QDebug>

MoveTableModel::MoveTableModel(QObject* parent) : QStandardItemModel(0, 2, parent)
{
	//m_move =  QList<MoveTableItem>();
	m_count = 0;
	m_white = true;
	setHeaderData(0, Qt::Horizontal, i18n("White"));
	setHeaderData(1, Qt::Horizontal, i18n("Black"));
}

void MoveTableModel::clean()
{
	clear();
	m_count = 0;
	m_white = true;
	setHeaderData(0, Qt::Horizontal, i18n("White"));
	setHeaderData(1, Qt::Horizontal, i18n("Black"));
}

void MoveTableModel::removeLastMove()
{
	if(m_white){
		m_count--;
	}
	m_white = !m_white;
	
	takeItem(m_count, m_white? 0:1);
}

/*
int MoveTableModel::rowCount(const QModelIndex& parent) const
{
  return m_move.size()/2 + m_move.size()%2;
}

int MoveTableModel::columnCount(const QModelIndex& parent) const
{
  return 2;
}

QVariant MoveTableModel::data(const QModelIndex& index, int role) const
{
  int pos = index.row()*2 + index.column();
  if(!index.isValid() || m_move.size() <= pos)
    return QVariant();
  return m_move.at(pos);
}
*/
/*
QVariant MoveTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if(orientation == Qt::Horizontal){
    if(section == 0)
      return i18n("White");
    else
      return i18n("Black");
  }else{
    return section + 1;
  }
}
*/
void MoveTableModel::addMove(const QString& move, ChessUtilities::Pieces type)
{ 
	//this->appendRow(new MoveTableItem(white, 0, move));
	this->setItem(m_count, m_white? 0:1, new MoveTableItem(m_white, type, move));
	if(!m_white)
		m_count++;
	m_white = !m_white;
}
