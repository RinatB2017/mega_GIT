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
#include <QDesktopServices>
#include <QMessageBox>
#include <QTableView>
#include <QSqlQuery>
#include <QFile>
#include <QDir>
#include <QUrl>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "reportstring.hpp"
#include "openreports.h"
#include "database.hpp"
#include "myreport.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
MyReport::MyReport(QWidget *parent) :
    QWidget(parent),
    database(0)
{
    init();
}
//--------------------------------------------------------------------------------
MyReport::~MyReport()
{
    if(database)
    {
        database->close();
        database->deleteLater();
    }
}
//--------------------------------------------------------------------------------
void MyReport::connect_log(void)
{
    if(parentWidget())
    {
        // qDebug() << "parent is true";
        connect(this, SIGNAL(info(QString)),  parentWidget(), SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parentWidget(), SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parentWidget(), SIGNAL(error(QString)));
    }
    else
    {
        // qDebug() << "parent is false";
        connect(this, SIGNAL(info(QString)),  this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)), this, SLOT(log(QString)));
    }
}
//--------------------------------------------------------------------------------
void MyReport::log(const QString &data)
{
#ifdef QT_DEBUG
    qDebug() << data;
#else
    Q_UNUSED(data)
#endif
}
//--------------------------------------------------------------------------------
void MyReport::init(void)
{
    connect_log();
}
//--------------------------------------------------------------------------------
bool MyReport::open_database(const QString &driver_name,
                             const QString &database_name)
{
    emit info(QString("Open driver %1, database_name %2")
              .arg(driver_name)
              .arg(database_name));

    database = new Database(driver_name, database_name);

    bool ok = database->open();
    if(!ok)
    {
        emit error(QString("db %1 not open!").arg(database_name));
        emit error(database->get_lastError());
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
void MyReport::close_database(const QString &database_name)
{
    if(!database->isValid()) return;
    database->close();
    database->removeDatabase(database_name);
}
//--------------------------------------------------------------------------------
bool MyReport::add_report_to_db(const QString &report_name,
                                const QString &report_descrip,
                                const QString &data)
{
    bool ok;
    QSqlQuery query;

    emit info("add_report_to_db");
    database->drop_table("report");
    ok = query.prepare("CREATE TABLE report ("
                           " report_id SERIAL PRIMARY KEY, "
                           " report_name TEXT, "
                           " report_descrip TEXT NOT NULL, "
                           " report_grade INTEGER NOT NULL DEFAULT 0, "
                           " report_source TEXT) ");
    if(ok == false)
    {
        emit error("add_report_to_db: prepare");
        emit error(database->get_lastError());
        return false;
    }
    ok = query.exec();
    if(ok == false)
    {
        emit error("add_report_to_db: SQL not exec!");
        return false;
    }
    query.prepare("INSERT INTO report (report_id, report_name, report_descrip, report_grade, report_source) "
                      "VALUES (:report_id, :report_name, :report_descrip, :report_grade, :report_source)");
    query.bindValue(":report_id", 0);
    query.bindValue(":report_name", report_name);
    query.bindValue(":report_descrip", report_descrip);
    query.bindValue(":report_grade", 0);
    query.bindValue(":report_source", data);

    ok = query.exec();
#ifdef QT_DEBUG
    qDebug() << "query.exec() return" << ok;
#endif
    return ok;
}
//--------------------------------------------------------------------------------
void MyReport::add_data_to_table(const QString &table_name,
                                 const QString &data_measure_name,
                                 const QString &deviation,
                                 const QString &data_measure_v1,
                                 const QString &data_measure_v2,
                                 const QString &data_measure_v3,
                                 const QString &data_measure_v4,
                                 const QString &data_measure_v5,
                                 const QString &data_measure_v6,
                                 const QString &data_measure_v7)
{
    QSqlQuery query;

    query.clear();
    query.prepare(QString("INSERT INTO %1 ("
                          "measure_name, "
                          "deviation, "
                          "measure_v1, "
                          "measure_v2, "
                          "measure_v3, "
                          "measure_v4, "
                          "measure_v5, "
                          "measure_v6, "
                          "measure_v7) "
                          "VALUES ("
                          ":measure_name, "
                          ":deviation, "
                          ":measure_v1, "
                          ":measure_v2, "
                          ":measure_v3, "
                          ":measure_v4, "
                          ":measure_v5, "
                          ":measure_v6, "
                          ":measure_v7) ")
                  .arg(table_name));
    query.bindValue(":measure_name", data_measure_name);
    query.bindValue(":deviation", deviation);
    query.bindValue(":measure_v1", data_measure_v1);
    query.bindValue(":measure_v2", data_measure_v2);
    query.bindValue(":measure_v3", data_measure_v3);
    query.bindValue(":measure_v4", data_measure_v4);
    query.bindValue(":measure_v5", data_measure_v5);
    query.bindValue(":measure_v6", data_measure_v6);
    query.bindValue(":measure_v7", data_measure_v7);
    query.exec();
}
//--------------------------------------------------------------------------------
bool MyReport::create_data_table(const QString &table_name)
{
    QSqlQuery query;
    bool ok;

    query.prepare(QString("CREATE TABLE %1 ("
                          "measure_name TEXT, "
                          "deviation TEXT, "
                          "measure_v1 TEXT, "
                          "measure_v2 TEXT, "
                          "measure_v3 TEXT, "
                          "measure_v4 TEXT, "
                          "measure_v5 TEXT, "
                          "measure_v6 TEXT, "
                          "measure_v7 TEXT) ")
                  .arg(table_name));
    ok = query.exec();
    if(ok == false)
    {
        emit error(QString(tr("table '%1' not created"))
                   .arg(table_name));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool MyReport::save_report_to_xml(const QString &filename,
                                  const QString &report_txt)
{
    QFile xml_file(filename);
    if (xml_file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        xml_file.write(report_txt.toLocal8Bit());
        xml_file.close();
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
QString MyReport::load_report_from_xml(const QString &filename)
{
    QString temp;
    QFile xml_file(filename);
    if (xml_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        temp = xml_file.read(1000000);
        xml_file.close();
        return temp;
    }
    return "";
}
//--------------------------------------------------------------------------------
void MyReport::view(int serno, int year)
{
    ReportString *report = new ReportString(this);

    int cnt_tables = database->get_count_tables();
    if((cnt_tables != 11) && (cnt_tables != 12))
    {
        //QMessageBox::critical(this, tr("Ошибка!"), QString(tr("Таблицы не найдены! (count %1)")).arg(cnt_tables));
        //clean_database_report();
    }

    report->set_title(tr("Протокол предварительных испытаний"));
    report->set_name(tr("Протокол"));
    report->set_description(tr("нет описания"));
    report->set_multimeter_serno(serno);

    report->set_font_name("Times New Roman");
    report->set_font_size(8);
    report->set_font_weight("normal");

    report->set_topmargin(10);
    report->set_bottommargin(15);
    report->set_rightmargin(10);
    report->set_leftmargin(10);

    // загрузка XML-report
#if 0
    QString temp;
    bool ok = report->load_xml(":/report/report.xml", &temp);
    if(!ok)
    {
        emit error("load_xml FAILED!");
        return;
    }
#else
    QString temp = report->prepare_report_xml();
#endif

    save_report_to_xml("out.xml", temp);

    if(add_report_to_db("my first report", "my_descr", temp) == false)
    {
        emit error("add_report_to_db failed!");
        return;
    }

    QSqlDatabase db = database->get_db();
    if(!db.isValid())
    {
        emit error("DB failed!");
    }
    orReport f_report("my first report", db);
    if(f_report.isValid())
    {
        emit info("report is OK");

        QString pdf_filename = QString(REPORT_NAME_PDF)
                .arg(QDir::currentPath())
                .arg(serno)
                .arg(year);
        bool ok = f_report.exportToPDF(pdf_filename);
        emit info(QString("Export to %1 is %2")
                  .arg(pdf_filename)
                  .arg(ok ? "true" : "false"));
        QDesktopServices::openUrl(QUrl::fromLocalFile(pdf_filename));
    }
    else
    {
        emit error("report is FAILED");
        f_report.reportError(this);
    }

    report->deleteLater();
}
//--------------------------------------------------------------------------------
void MyReport::clean_database_report(void)
{
    QPixmap pixmap(":/logo/pinguin.png");
    MyWaitSplashScreen *wait_splash = new MyWaitSplashScreen(pixmap, 4);
    wait_splash->show();
    wait_splash->showMessage(QObject::tr("Подождите ..."));

    wait_splash->showMessage(QObject::tr("Очистка таблиц ..."));
    database->drop_table("data_report_UDC");
    database->drop_table("data_report_IDC");

    wait_splash->showMessage(QObject::tr("Создание таблиц ..."));
    create_data_table("data_report_UDC");
    create_data_table("data_report_IDC");

    wait_splash->showMessage(QObject::tr("Обнуление таблиц UDC ..."));
    add_data_to_table("data_report_UDC", CONTROL_1V_TEXT,  "", "", "", "", "", "", "", "");
    add_data_to_table("data_report_UDC", CONTROL_15V_TEXT, "", "", "", "", "", "", "", "");
    add_data_to_table("data_report_UDC", CONTROL_30V_TEXT, "", "", "", "", "", "", "", "");
    add_data_to_table("data_report_UDC", CONTROL_60V_TEXT, "", "", "", "", "", "", "", "");

    wait_splash->showMessage(QObject::tr("Обнуление таблиц IDC ..."));
    add_data_to_table("data_report_IDC", CONTROL_01A_TEXT, "", "", "", "", "", "", "", "");
    add_data_to_table("data_report_IDC", CONTROL_5A_TEXT,  "", "", "", "", "", "", "", "");
    add_data_to_table("data_report_IDC", CONTROL_10A_TEXT, "", "", "", "", "", "", "", "");
    add_data_to_table("data_report_IDC", CONTROL_30A_TEXT, "", "", "", "", "", "", "", "");

    wait_splash->finish(0);
    wait_splash->deleteLater();
}
//--------------------------------------------------------------------------------
bool MyReport::update_data(const QString &table_name,
                           const QString &measure_string,
                           int row,
                           const QString &value)
{
    QSqlQuery query;
    QString data_name;

    switch(row)
    {
    case 0:  data_name = "measure_v1"; break;
    case 1:  data_name = "measure_v2"; break;
    case 2:  data_name = "measure_v3"; break;
    case 3:  data_name = "measure_v4"; break;
    case 4:  data_name = "measure_v5"; break;
    case 5:  data_name = "measure_v6"; break;
    case 6:  data_name = "measure_v7"; break;
    default: data_name = "measure_v1"; break;
    }

    query.clear();
    query.prepare(QString("UPDATE %1 SET %2 = '%3' WHERE measure_name = '%4'")
                  .arg(table_name)
                  .arg(data_name)
                  .arg(value)
                  .arg(measure_string));
    return query.exec();
}
//--------------------------------------------------------------------------------
int MyReport::get_count_tables(void)
{
    return database->get_count_tables();
}
//--------------------------------------------------------------------------------
QString MyReport::get_lastError(void)
{
    return database->get_lastError();
}
//--------------------------------------------------------------------------------
