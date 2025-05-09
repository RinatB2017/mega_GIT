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
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include <QTableView>
#include <QtSql>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent = nullptr);
    virtual ~MainBox();

private slots:
    void test(void);

private:
    Ui::MainBox *ui;

    void init(void);
    void createTestBar(void);

    QSqlDatabase db;
    int  open_database(const QString &driver_name, const QString &database_name);
    void view_table(const QString &table_name);
    bool drop_table(const QString &table_name);
    void close_database(const QString &database_name);

    bool execTableBuild(const QString &qryStr, QTextStream &out);
    bool odbcSanityCheck(QSqlDatabase &db, QString &dbServer);
    void displayNotSupportedMesg(QSqlDatabase &db, QTextStream &out);
    bool buildTable(QSqlDatabase &db, QTextStream &out, QString &dbServer);
    void create_report_tables(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
