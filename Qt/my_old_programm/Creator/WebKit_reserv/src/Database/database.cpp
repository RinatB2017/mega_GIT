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
#include <QSqlQueryModel>
#include <QHeaderView>
#include <QTableView>
#include <QSqlQuery>
#include <QVariant>
//--------------------------------------------------------------------------------
#include "database.hpp"
//--------------------------------------------------------------------------------
Database::Database(const QString &driver_name,
                   const QString &database_name,
                   QObject *parent) :
    QObject(parent),
    driver_name(driver_name),
    database_name(database_name)
{

}
//--------------------------------------------------------------------------------
bool Database::open(void)
{
    if(db.databaseName() != database_name)
    {
        db = QSqlDatabase::addDatabase(driver_name);
        db.setDatabaseName(database_name);
    }
    bool ok = db.open();
    if(ok == false)
    {
        emit error(QString(tr("База данных %1 не открыта!")).arg(database_name));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
void Database::close(void)
{
    if(!db.isValid())
        return;

    db.close();
    db.removeDatabase(database_name);
}
//--------------------------------------------------------------------------------
void Database::view(const QString &query)
{
    if(!db.isValid())
        return;

    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(QString(query));

    QTableView *view = new QTableView;
    view->setWindowTitle("view table");
    view->setWindowIcon(QIcon(QLatin1String(":/mainwindow/computer.png")));
    view->setMinimumSize(1024, 600);

    view->setModel(model);

    QHeaderView *header = new QHeaderView(Qt::Horizontal);
    header->setResizeMode(QHeaderView::ResizeToContents);
    view->setHorizontalHeader(header);

    view->show();
}
//--------------------------------------------------------------------------------
bool Database::sql(const QString &query)
{
    if(!db.isValid())
        return false;

    QSqlQuery sql;
    bool result = sql.exec(query);
    return result;
}
//--------------------------------------------------------------------------------
