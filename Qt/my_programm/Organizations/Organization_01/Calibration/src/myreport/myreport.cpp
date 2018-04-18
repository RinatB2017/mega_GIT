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
#   include <QDebug>
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
    MyWidget(parent)
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
void MyReport::init(void)
{

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
                                 const QString &data_measure_v6)
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
                          "measure_v6) "
                          "VALUES ("
                          ":measure_name, "
                          ":deviation, "
                          ":measure_v1, "
                          ":measure_v2, "
                          ":measure_v3, "
                          ":measure_v4, "
                          ":measure_v5, "
                          ":measure_v6) ")
                  .arg(table_name));
    query.bindValue(":measure_name", data_measure_name);
    query.bindValue(":deviation", deviation);
    query.bindValue(":measure_v1", data_measure_v1);
    query.bindValue(":measure_v2", data_measure_v2);
    query.bindValue(":measure_v3", data_measure_v3);
    query.bindValue(":measure_v4", data_measure_v4);
    query.bindValue(":measure_v5", data_measure_v5);
    query.bindValue(":measure_v6", data_measure_v6);
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
                          "measure_v6 TEXT) ")
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
        clean_database_report();
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

    QString temp = report->prepare_report_xml();
    //TODO
    //temp.replace("<rect>", "");
    //temp.replace("</rect>", "");
    //qDebug() << temp;
    //---
    save_report_to_xml("out.xml", temp);

    if(add_report_to_db("my first report", "my_descr", temp) == false)
    {
        return;
    }

    orReport f_report("my first report", database->get_db());
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
    QPixmap pixmap(":/logo/logo.png");
    MyWaitSplashScreen *wait_splash = new MyWaitSplashScreen(pixmap, 12);
    wait_splash->show();
    wait_splash->showMessage(QObject::tr("Подождите ..."));

    wait_splash->showMessage(QObject::tr("Очистка таблиц ..."));
    database->drop_table("data_report_UAC");
    database->drop_table("data_report_UDC");
    database->drop_table("data_report_IAC");
    database->drop_table("data_report_IDC");
    database->drop_table("data_report_R");
    database->drop_table("data_report_L");
    database->drop_table("data_report_C");
    database->drop_table("data_report_FREQ");
    database->drop_table("data_report_hi_power");
    database->drop_table("data_report_lo_power");
    database->drop_table("data_report_EMPTY");

    wait_splash->showMessage(QObject::tr("Создание таблиц ..."));
    create_data_table("data_report_UAC");
    create_data_table("data_report_UDC");
    create_data_table("data_report_IAC");
    create_data_table("data_report_IDC");
    create_data_table("data_report_R");
    create_data_table("data_report_L");
    create_data_table("data_report_C");
    create_data_table("data_report_FREQ");
    create_data_table("data_report_hi_power");
    create_data_table("data_report_lo_power");
    create_data_table("data_report_EMPTY");

    add_data_to_table("data_report_EMPTY", "", "", "", "", "", "", "", "");

    wait_splash->showMessage(QObject::tr("Обнуление таблиц UDC ..."));
    add_data_to_table("data_report_UDC", CONTROL_UDC_10mV_TEXT, tr("0.04 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UDC", CONTROL_UDC_minus_10mV_TEXT, tr("0.04 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UDC", CONTROL_UDC_170mV_TEXT, tr("0.34 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UDC", CONTROL_UDC_minus_170mV_TEXT, tr("0.34 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UDC", CONTROL_UDC_0_5V_TEXT, tr("0.3 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UDC", CONTROL_UDC_minus_0_5V_TEXT, tr("0.3 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UDC", CONTROL_UDC_1V_TEXT, tr("0.5 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UDC", CONTROL_UDC_1_5V_TEXT, tr("0.7 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UDC", CONTROL_UDC_1_7V_TEXT, tr("0.9 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UDC", CONTROL_UDC_minus_1_7V_TEXT, tr("0.9 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UDC", CONTROL_UDC_17_0V_TEXT, tr("9 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UDC", CONTROL_UDC_170V_TEXT, tr("90 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UDC", CONTROL_UDC_1000V_TEXT, tr("2.8 В"), "", "", "", "", "", "");
    add_data_to_table("data_report_UDC", CONTROL_UDC_minus_1000V_TEXT, tr("2.8 В"), "", "", "", "", "", "");
    //add_data_to_table("data_report_UDC", "", "", "", "", "", "", "", ""); // пустая строка
    //add_data_to_table("data_report_UDC", "", "", "", "", "", "", "", ""); // пустая строка

    wait_splash->showMessage(QObject::tr("Обнуление таблиц UAC ..."));
    add_data_to_table("data_report_UAC", CONTROL_UAC_17mV_20kHz_TEXT, tr("0.96 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UAC", CONTROL_UAC_190mV_1kHz_TEXT, tr("1.2 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UAC", CONTROL_UAC_190mV_50kHz_TEXT, tr("4.78 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UAC", CONTROL_UAC_190mV_100kHz_TEXT, tr("4.78 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UAC", CONTROL_UAC_0_5V_1kHz_TEXT, tr("2.8 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UAC", CONTROL_UAC_1V_20kHz_TEXT, tr("4 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UAC", CONTROL_UAC_1V_1kHz_TEXT, tr("4 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UAC", CONTROL_UAC_1V_20kHz_TEXT, tr("8.8 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UAC", CONTROL_UAC_1V_50kHz_TEXT, tr("28 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UAC", CONTROL_UAC_1V_100kHz_TEXT, tr("28 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UAC", CONTROL_UAC_1_7V_1kHz_TEXT, tr("5.6 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UAC", CONTROL_UAC_10V_1kHz_TEXT, tr("32 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UAC", CONTROL_UAC_10V_20kHz_TEXT, tr("64 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UAC", CONTROL_UAC_10V_50kHz_TEXT, tr("280 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UAC", CONTROL_UAC_10V_100kHz_TEXT, tr("280 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_UAC", CONTROL_UAC_100V_1kHz_TEXT, tr("0.4 В"), "", "", "", "", "", "");
    add_data_to_table("data_report_UAC", CONTROL_UAC_100V_20kHz_TEXT, tr("1.6 В"), "", "", "", "", "", "");
    add_data_to_table("data_report_UAC", CONTROL_UAC_100V_100kHz_TEXT, tr("2.16 В"), "", "", "", "", "", "");
    add_data_to_table("data_report_UAC", CONTROL_UAC_170V_1kHz_TEXT, tr("0.72 В"), "", "", "", "", "", "");
    add_data_to_table("data_report_UAC", CONTROL_UAC_400V_1kHz_TEXT, tr("2.4 В"), "", "", "", "", "", "");

    wait_splash->showMessage(QObject::tr("Обнуление таблиц IDC ..."));
    add_data_to_table("data_report_IDC", CONTROL_IDC_0_5mkA_TEXT, tr("0.8 мкА"), "", "", "", "", "", "");
    add_data_to_table("data_report_IDC", CONTROL_IDC_100mkA_TEXT, tr("1.6 мкА"), "", "", "", "", "", "");
    add_data_to_table("data_report_IDC", CONTROL_IDC_minus_100mkA_TEXT, tr("1.6 мкА"), "", "", "", "", "", "");
    add_data_to_table("data_report_IDC", CONTROL_IDC_1mA_TEXT, tr("0.0048 мА"), "", "", "", "", "", "");
    add_data_to_table("data_report_IDC", CONTROL_IDC_minus_1mA_TEXT, tr("0.0048 мА"), "", "", "", "", "", "");
    add_data_to_table("data_report_IDC", CONTROL_IDC_10mA_TEXT, tr("0.016 мА"), "", "", "", "", "", "");
    add_data_to_table("data_report_IDC", CONTROL_IDC_minus_10mA_TEXT, tr("0.016 мА"), "", "", "", "", "", "");
    add_data_to_table("data_report_IDC", CONTROL_IDC_100mA_TEXT, tr("0.16 мА"), "", "", "", "", "", "");
    add_data_to_table("data_report_IDC", CONTROL_IDC_1000mA_TEXT, tr("1.6 мА"), "", "", "", "", "", "");
    add_data_to_table("data_report_IDC", CONTROL_IDC_minus_1000mA_TEXT, tr("1.6 мА"), "", "", "", "", "", "");
    add_data_to_table("data_report_IDC", CONTROL_IDC_19A_TEXT, tr("0.08 А"), "", "", "", "", "", "");
    add_data_to_table("data_report_IDC", CONTROL_IDC_minus_19A_TEXT, tr("0.08 А"), "", "", "", "", "", "");

    wait_splash->showMessage(QObject::tr("Обнуление таблиц IAC ..."));
    add_data_to_table("data_report_IAC", CONTROL_IAC_1mA_1_kHz_TEXT, tr("0.0048 мА"), "", "", "", "", "", "");
    add_data_to_table("data_report_IAC", CONTROL_IAC_10mA_1_kHz_TEXT, tr("0.044 мА"), "", "", "", "", "", "");
    add_data_to_table("data_report_IAC", CONTROL_IAC_100mA_1_kHz_TEXT, tr("0.32 мА"), "", "", "", "", "", "");
    add_data_to_table("data_report_IAC", CONTROL_IAC_1000mA_1_kHz_TEXT, tr("3.2 мА"), "", "", "", "", "", "");
    add_data_to_table("data_report_IAC", CONTROL_IAC_10A_1_kHz_TEXT, tr("0.24 А"), "", "", "", "", "", "");

    wait_splash->showMessage(QObject::tr("Обнуление таблиц R ..."));
    add_data_to_table("data_report_R", CONTROL_R_0_Om_TEXT, tr("0.08 Ом"), "", "", "", "", "", "");
    add_data_to_table("data_report_R", CONTROL_R_10_Om_TEXT, tr("0.11 Ом"), "", "", "", "", "", "");
    add_data_to_table("data_report_R", CONTROL_R_100_Om_TEXT, tr("0.36 Ом"), "", "", "", "", "", "");
    add_data_to_table("data_report_R", CONTROL_R_190_Om_TEXT, tr("0.61 Ом"), "", "", "", "", "", "");
    add_data_to_table("data_report_R", CONTROL_R_1_kOm_TEXT, tr("1.7 Ом"), "", "", "", "", "", "");
    add_data_to_table("data_report_R", CONTROL_R_1_9_kOm_TEXT, tr("3.1 Ом"), "", "", "", "", "", "");
    add_data_to_table("data_report_R", CONTROL_R_10_kOm_TEXT, tr("12 Ом"), "", "", "", "", "", "");
    add_data_to_table("data_report_R", CONTROL_R_100_kOm_TEXT, tr("120 Ом"), "", "", "", "", "", "");
    add_data_to_table("data_report_R", CONTROL_R_1_MOm_TEXT, tr("0.0032 МОм"), "", "", "", "", "", "");

    wait_splash->showMessage(QObject::tr("Обнуление таблиц C ..."));
    add_data_to_table("data_report_C", CONTROL_C_0_pF_TEXT, tr("2.4 пФ"), "", "", "", "", "", "");
    add_data_to_table("data_report_C", CONTROL_C_100_pF_TEXT, tr("4 пФ"), "", "", "", "", "", "");
    add_data_to_table("data_report_C", CONTROL_C_1000_pF_TEXT, tr("40 пФ"), "", "", "", "", "", "");
    add_data_to_table("data_report_C", CONTROL_C_10000_pF_TEXT, tr("400 пФ"), "", "", "", "", "", "");
    add_data_to_table("data_report_C", CONTROL_C_0_1_mkF_TEXT, tr("0.004 мкФ"), "", "", "", "", "", "");
    add_data_to_table("data_report_C", CONTROL_C_1_mkF_TEXT, tr("0.04 мкФ"), "", "", "", "", "", "");
    add_data_to_table("data_report_C", CONTROL_C_10_mkF_TEXT, tr("0.4 мкФ"), "", "", "", "", "", "");
    add_data_to_table("data_report_C", CONTROL_C_1000_mkF_TEXT, tr("Возможность измерения"), "", "", "", "", "", "");

    wait_splash->showMessage(QObject::tr("Обнуление таблиц L ..."));
    add_data_to_table("data_report_L", CONTROL_L_0_mH_TEXT, tr("0.016 мГн"), "", "", "", "", "", "");
    add_data_to_table("data_report_L", CONTROL_L_1_mH_TEXT, tr("0.04 мГн"), "", "", "", "", "", "");
    add_data_to_table("data_report_L", CONTROL_L_10_mH_TEXT, tr("0.4 мГн"), "", "", "", "", "", "");
    add_data_to_table("data_report_L", CONTROL_L_100_mH_TEXT, tr("4 мГн"), "", "", "", "", "", "");

    wait_splash->showMessage(QObject::tr("Обнуление таблиц FREQ ..."));
    add_data_to_table("data_report_FREQ", "", tr("0.04 кГц"), "", "", "", "", "", "");

    //повышенное напряжение
    wait_splash->showMessage(QObject::tr("Обнуление таблиц HP ..."));
    add_data_to_table("data_report_hi_power", CONTROL_HI_UDC_1V_TEXT, tr("0.6 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_hi_power", CONTROL_HI_UDC_10V_TEXT, tr("6 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_hi_power", CONTROL_HI_UAC_1V_1kHz_TEXT, tr("5 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_hi_power", CONTROL_HI_UAC_10V_1kHz_TEXT, tr("0.04 В"), "", "", "", "", "", "");

    add_data_to_table("data_report_hi_power", CONTROL_HI_R_1kOm_TEXT, tr("0.0021 кОм"), "", "", "", "", "", "");
    add_data_to_table("data_report_hi_power", CONTROL_HI_R_100kOm_TEXT, tr("0.15 кОм"), "", "", "", "", "", "");

    add_data_to_table("data_report_hi_power", CONTROL_HI_IDC_10A_TEXT, tr("0.06 А"), "", "", "", "", "", "");
    add_data_to_table("data_report_hi_power", CONTROL_HI_FREQ_1MHz_TEXT, tr("0.0005 МГц"), "", "", "", "", "", "");
    add_data_to_table("data_report_hi_power", CONTROL_HI_C_1mkF_TEXT, tr("0.05 мкФ"), "", "", "", "", "", "");
    add_data_to_table("data_report_hi_power", CONTROL_HI_L_10mH_TEXT, tr("0.5 мГн"), "", "", "", "", "", "");

    //пониженное напряжение
    wait_splash->showMessage(QObject::tr("Обнуление таблиц LP ..."));
    add_data_to_table("data_report_lo_power", CONTROL_LO_UDC_1V_TEXT, tr("0.6 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_lo_power", CONTROL_LO_UDC_10V_TEXT, tr("6 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_lo_power", CONTROL_LO_UAC_1V_1kHz_TEXT, tr("5 мВ"), "", "", "", "", "", "");
    add_data_to_table("data_report_lo_power", CONTROL_LO_UAC_10V_1kHz_TEXT, tr("0.04 В"), "", "", "", "", "", "");

    add_data_to_table("data_report_lo_power", CONTROL_LO_R_1kOm_TEXT, tr("0.0021 кОм"), "", "", "", "", "", "");
    add_data_to_table("data_report_lo_power", CONTROL_LO_R_100kOm_TEXT, tr("0.15 кОм"), "", "", "", "", "", "");

    add_data_to_table("data_report_lo_power", CONTROL_LO_IDC_10A_TEXT, tr("0.06 А"), "", "", "", "", "", "");
    add_data_to_table("data_report_lo_power", CONTROL_LO_FREQ_1MHz_TEXT, tr("0.0005 МГц"), "", "", "", "", "", "");
    add_data_to_table("data_report_lo_power", CONTROL_LO_C_1mkF_TEXT, tr("0.05 мкФ"), "", "", "", "", "", "");
    add_data_to_table("data_report_lo_power", CONTROL_LO_L_10mH_TEXT, tr("0.5 мГн"), "", "", "", "", "", "");


#if 0
    qDebug() << "test query is" << update_data("data_report_UDC", CONTROL_UDC_1V_TEXT, 2, "1.666 В");
#endif

    wait_splash->finish(0);
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
void MyReport::updateText(void)
{

}
//--------------------------------------------------------------------------------
