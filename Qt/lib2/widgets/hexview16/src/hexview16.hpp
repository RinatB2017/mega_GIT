/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#ifndef HEXVIEW16_HPP
#define HEXVIEW16_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#define MAX_ROW 0x1000
#define MAX_COL 0x10
//--------------------------------------------------------------------------------
class HexView16 : public QWidget
{
    Q_OBJECT

public:
    explicit HexView16(QWidget *parent = nullptr);
    ~HexView16();

    void set(uint16_t address, uint16_t value);
    uint16_t get(uint16_t address);

private:
    QPointer<QTableView> tv;
    QPointer<QStandardItemModel> model;
    uint16_t buf[MAX_ROW*MAX_COL + 1] = { 0 };
    QStandardItem *items[MAX_ROW*MAX_COL + 1] = { nullptr };

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // HEXVIEW16_HPP
