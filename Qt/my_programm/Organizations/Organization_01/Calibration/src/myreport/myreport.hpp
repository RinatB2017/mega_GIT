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
#ifndef MYREPORT_HPP
#define MYREPORT_HPP
//--------------------------------------------------------------------------------
#include <QWidget>

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class Database;
//--------------------------------------------------------------------------------
class MyReport : public MyWidget
{
    Q_OBJECT
public:
    MyReport(QWidget *parent = 0);
    ~MyReport();

    void view(int serno, int year);
    void clean_database_report(void);

    bool update_data(const QString &table_name,
                     const QString &measure_string,
                     int row,
                     const QString &value);
    bool open_database(const QString &driver_name,
                      const QString &database_name);
    void close_database(const QString &database_name);
    int get_count_tables(void);
    QString get_lastError(void);

private:
    Database *database = 0;

    void init(void);

    bool add_report_to_db(const QString &report_name,
                          const QString &report_descrip,
                          const QString &data);
    void add_data_to_table(const QString &table_name,
                           const QString &data_measure_name,
                           const QString &deviation,
                           const QString &data_measure_v1,
                           const QString &data_measure_v2,
                           const QString &data_measure_v3,
                           const QString &data_measure_v4,
                           const QString &data_measure_v5,
                           const QString &data_measure_v6);
    bool create_data_table(const QString &table_name);
    bool save_report_to_xml(const QString &filename,
                            const QString &report_txt);
    QString load_report_from_xml(const QString &filename);

    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // MYREPORT_HPP
