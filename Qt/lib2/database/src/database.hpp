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
#ifndef DATABASE_HPP
#define DATABASE_HPP
//--------------------------------------------------------------------------------
#include <QSqlDatabase>
#include <QStringList>
#include <QObject>
//--------------------------------------------------------------------------------
class QTableView;
//--------------------------------------------------------------------------------
class Database : public QObject
{
    Q_OBJECT
public:
    Database(const QString &driver_name,
                      const QString &database_name,
                      QObject *parent = nullptr);
    virtual ~Database();

    bool isValid(void);
    QString get_lastError(void);
    void removeDatabase(const QString &database_name);
    bool drop_table(const QString &table_name);
    int get_count_tables(void);
    QSqlDatabase get_db(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

public slots:
    bool open(void);
    void close(void);
    void view(const QString &query);
    bool sql(const QString &query);
    QStringList tables(void);
    bool isOpen(void);
    QStringList list_drivers(void);

private slots:
    void log(const QString &text);

private:
    QSqlDatabase db;
    QString driver_name;
    QString database_name;
    QTableView *table_view = nullptr;
};
//--------------------------------------------------------------------------------
#endif // DATABASE_HPP
