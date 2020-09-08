/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#ifndef TABLEVIEW_HPP
#define TABLEVIEW_HPP
//--------------------------------------------------------------------------------
#include <QSqlDatabase>
#include <QTableView>
//--------------------------------------------------------------------------------
class QSqlQueryModel;
//--------------------------------------------------------------------------------
class TableView : public QTableView
{
    Q_OBJECT
public:
    TableView(QWidget *parent = nullptr);
    ~TableView();

    void init(const QString &query);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

private slots:
    void log(const QString &text);
    void doubleClicked(QModelIndex index);

private:
    QString sql_query;
    QSqlDatabase db;
    QString driver_name;
    QString database_name;
    QSqlQueryModel *model;
};
//--------------------------------------------------------------------------------
#endif // TABLEVIEW_HPP
