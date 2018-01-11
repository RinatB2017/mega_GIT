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
#include <QSqlError>
#include <QSqlQuery>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "database.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
Database::Database(const QString &driver_name,
                   const QString &database_name,
                   QObject *parent) :
    QObject(parent),
    driver_name(driver_name),
    database_name(database_name),
    table_view(0)
{
    if(parent)
    {
        connect(this, SIGNAL(info(QString)),  parent, SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parent, SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parent, SIGNAL(error(QString)));
        connect(this, SIGNAL(trace(QString)), parent, SIGNAL(trace(QString)));
    }
    else
    {
        connect(this, SIGNAL(info(QString)),  this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(trace(QString)), this, SLOT(log(QString)));
    }
}
//--------------------------------------------------------------------------------
Database::~Database()
{
    if(table_view)
    {
        table_view->close();
        delete table_view;
    }
}
//--------------------------------------------------------------------------------
void Database::log(const QString &text)
{
#ifdef QT_DEBUG
    qDebug() << text;
#else
    Q_UNUSED(text);
#endif
}
//--------------------------------------------------------------------------------
QStringList Database::list_drivers(void)
{
    return QSqlDatabase::drivers();
}
//--------------------------------------------------------------------------------
bool Database::open(void)
{
    if(db.databaseName() != database_name)
    {
        if(QSqlDatabase::contains(QSqlDatabase::defaultConnection))
        {
            db = QSqlDatabase::database();
        }
        else
        {
            db = QSqlDatabase::addDatabase(driver_name);
            db.setDatabaseName(database_name);
        }
    }
    bool ok = db.open();
    if(ok == false)
    {
        emit error(QString(tr("База данных %1 не открыта! Error: %2"))
                   .arg(database_name)
                   .arg(db.lastError().text()));
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

    table_view = new QTableView;
    table_view->setWindowTitle("view table");
    table_view->setMinimumSize(1024, 600);

    table_view->setModel(model);

    QHeaderView *header = new QHeaderView(Qt::Horizontal);
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    header->setResizeMode(QHeaderView::ResizeToContents);
#endif
    table_view->setHorizontalHeader(header);

    table_view->show();
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
QStringList Database::tables(void)
{
    return db.tables();
}
//--------------------------------------------------------------------------------
bool Database::isOpen(void)
{
    return db.isOpen();
}
//--------------------------------------------------------------------------------
bool Database::isValid(void)
{
    return db.isValid();
}
//--------------------------------------------------------------------------------
QString Database::get_lastError(void)
{
    return db.lastError().text();
}
//--------------------------------------------------------------------------------
void Database::removeDatabase(const QString &database_name)
{
    db.removeDatabase(database_name);
}
//--------------------------------------------------------------------------------
bool Database::drop_table(const QString &table_name)
{
    QSqlQuery sql;
    return sql.exec(QString("DROP TABLE %1").arg(table_name));
}
//--------------------------------------------------------------------------------
int Database::get_count_tables(void)
{
    return db.tables(QSql::Tables).count();
}
//--------------------------------------------------------------------------------
QSqlDatabase Database::get_db(void)
{
    return db;
}
//--------------------------------------------------------------------------------
