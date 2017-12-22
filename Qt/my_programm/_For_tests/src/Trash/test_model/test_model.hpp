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
#ifndef TEST_MODEL_HPP
#define TEST_MODEL_HPP
//--------------------------------------------------------------------------------
#include <QAbstractTableModel>
//--------------------------------------------------------------------------------
#define MAX_Y   5
#define MAX_X   5
//--------------------------------------------------------------------------------
class Test_model : public QAbstractTableModel
{
public:
    Test_model(QObject *parent = 0);

    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    void set(int row, int col, QVariant data);
    bool get(int row, int col, QVariant *data);

private:
    QVariant buf[MAX_Y][MAX_X] = {
        { 10, 20, 30, 40, 50 },
        { 11, 21, 31, 41, 51 },
        { 12, 22, 32, 42, 52 },
        { 13, 23, 33, 43, 53 },
        { 14, 24, 34, 44, 54 },
    };

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

};
//--------------------------------------------------------------------------------
#endif // TEST_MODEL_HPP
