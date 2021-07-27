/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
**                                                                              **
**     This program is free software: you can redistribute it and/or modify     **
**     it under the terms of the GNU General Public License as published by     **
**     the Free Software Foundation, either version 3 of the License, or        **
**     (at your option) any later version.                                      **
**                                                                              **
**     This program is distributed in the hope that it will be useful,          **
**     but WITHOUT ANY WARRANTY; without even the implied warranty of           **
**     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **
**     GNU General Public License for more details.                             **
**                                                                              **
**     You should have received a copy of the GNU General Public License        **
**     along with this program.  If not, see http://www.gnu.org/licenses/.      **
**                                                                              **
**********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#include "test_model.hpp"
//--------------------------------------------------------------------------------
Test_model::Test_model(QObject *parent) :
    QAbstractTableModel(parent)
{

}
//--------------------------------------------------------------------------------
int Test_model::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return MAX_X;
}
//--------------------------------------------------------------------------------
int Test_model::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return MAX_Y;
}
//--------------------------------------------------------------------------------
QVariant Test_model::data(const QModelIndex & index, int role) const
{
    if (index.isValid() && role == Qt::DisplayRole)
    {
        return buf[index.row()][index.column()];
    }
    return QVariant();
}
//--------------------------------------------------------------------------------
void Test_model::set(int row, int col, QVariant data)
{
    if(row >= MAX_Y)    return;
    if(col >= MAX_X)    return;
    buf[row][col] = data;
}
//--------------------------------------------------------------------------------
bool Test_model::get(int row, int col, QVariant *data)
{
    if(row >= MAX_Y)    return false;
    if(col >= MAX_X)    return false;
    *data = buf[row][col];
    return true;
}
//--------------------------------------------------------------------------------
