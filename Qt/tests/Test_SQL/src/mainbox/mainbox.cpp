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
#include <QToolButton>
#include <QTableView>
#include <QToolBar>
#include <QWidget>
#include <QStyle>
#include <QDebug>
#include <QtSql>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "defines.hpp"
#include "mainbox.hpp"

#include "builtinSqlFunctions.h"
#include "parameter.h"
#include "openreports.h"
#include "dbtools.h"

// return codes
#define EXIT_SUCCESS                  0
#define EXIT_ERROR_DB_DRIVER          1
#define EXIT_ERROR_DB_ENGINE          2
#define EXIT_ERROR_DB_LOGIN           3
#define EXIT_ERROR_DB_TABLE_BUILD     4
#define EXIT_ERROR_MISSING_URL        5
#define EXIT_ERROR_MISSING_USERNAME   6
#define EXIT_ERROR_MISSING_PASSWORD   7
#define EXIT_ERROR_MISSING_DB_ENGINE  8
#define EXIT_ERROR_BAD_ARGS           9
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox),
    parent(parent)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);
    createTestBar();

    //open_database("QSQLITE", "customdb");
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);
    if(mw ==nullptr)
    {
        return;
    }

    QToolBar *testbar = new QToolBar(this);
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       QIcon::fromTheme("system-run"), //qApp->style()->standardIcon(QStyle::SP_ComputerIcon),
                                       "test",
                                       "test");
    QToolButton *btn_open = add_button(testbar,
                                       new QToolButton(this),
                                       QIcon::fromTheme("document-open"), //qApp->style()->standardIcon(QStyle::SP_DialogOpenButton),
                                       "open",
                                       "open");
    QToolButton *btn_close = add_button(testbar,
                                        new QToolButton(this),
                                        QIcon::fromTheme("document-close"), //qApp->style()->standardIcon(QStyle::SP_DialogCloseButton),
                                        "close",
                                        "close");

    add_button(testbar, new QToolButton(this), qApp->style()->standardIcon(QStyle::SP_ArrowLeft), "abc", "xxx");

    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
    connect(btn_open, SIGNAL(clicked()), this, SLOT(test()));
    connect(btn_close, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
int MainBox::open_database(const QString &driver_name, const QString &database_name)
{
    emit info(QString("Open driver %1, database_name %2")
              .arg(driver_name)
              .arg(database_name));
    if(QSqlDatabase::contains(QSqlDatabase::defaultConnection))
    {
        db = QSqlDatabase::database();
    }
    else
    {
        db = QSqlDatabase::addDatabase(driver_name);
        db.setDatabaseName(database_name);
    }
    bool ok = db.open();
    if(!ok)
    {
        emit error(QString("db %1 not open!").arg(database_name));
        return 1;
    }
    return 0;
}
//--------------------------------------------------------------------------------
void MainBox::view_table(const QString &table_name)
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(QString("SELECT * FROM %1").arg(table_name));

    QTableView *view = new QTableView;
    view->setModel(model);
    view->setWindowIcon(QIcon(ICON_PROGRAMM));
    view->setMinimumSize(640, 480);
    view->show();
}
//--------------------------------------------------------------------------------
void MainBox::drop_table(const QString &table_name)
{
    QSqlQuery sql;
    sql.exec(QString("DROP TABLE %1").arg(table_name));
}
//--------------------------------------------------------------------------------
void MainBox::close_database(const QString &database_name)
{
    if(!db.isValid()) return;
    db.close();
    db.removeDatabase(database_name);
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool MainBox::execTableBuild(QString &qryStr, QTextStream &out)
{
    bool result;
    QSqlQuery qry;

    if(!(result = qry.exec(qryStr)))
    {
        out << "----------------------------" << endl;
        out << "Failed to build report table" << endl;
        out << "Error Number: " << qry.lastError().number() << endl;
        out << "Driver Error: " << qry.lastError().driverText() << endl;
        out << "Database Error: " << qry.lastError().databaseText() << endl;
    }
    else
        out << "Table built OK" << endl;

    return result;
}
//--------------------------------------------------------------------------------
bool MainBox::odbcSanityCheck(QSqlDatabase &db, QString &dbServer)
{
    bool result = true;

    // qDebug("Sanity check Server %s\n", dbServer.ascii());

    if ( db.driverName() == "QODBC" )
    {
        if( "odbc" == dbServer )    // An ODBC connection to an ODBC database
        {                           // danger of disappearing up own orifice
            dbServer = "QODBC";
            result = false;
        }
        else if ( "pgsql" == dbServer || "psql" == dbServer )
            dbServer = "QPSQL";
        else if ( "db2" == dbServer )
            dbServer = "QDB2";
        else if ( "ibase" == dbServer )
            dbServer = "QIBASE";
        else if ( "mysql" == dbServer )
            dbServer = "QMYSQL";
        else if ( "oracle" == dbServer )
            dbServer = "QOCI";
        else if ( "sqlite" == dbServer )
            dbServer = "QSQLITE";
        else if ( "sqlite2" == dbServer )
            dbServer = "QSQLITE2";
        else if ( "sybase" == dbServer )
            dbServer = "QTDS";
        else
            result = false;  // whatever type of server it is we don't know how to deal with it
    }

    return result;
}
//--------------------------------------------------------------------------------
void MainBox::displayNotSupportedMesg(QSqlDatabase &db, QTextStream &out)
{
    out << "Selected Driver [" << db.driverName() << "]" << endl;
    out << "Cannot build the required table - Do not know how to "              << endl;
    out << "specify an auto incrementing primary key for this database engine." << endl;
    out << "CREATE TABLE report"                                                << endl;
    out << "("                                                                  << endl;
    out << "  report_id integer NOT NULL PRIMARY KEY,"                          << endl;
    out << "  report_name TEXT,"                                                << endl;
    out << "  report_descrip TEXT NOT NULL,"                                    << endl;
    out << "  report_grade INTEGER NOT NULL DEFAULT 0,"                         << endl;
    out << "  report_source TEXT"                                               << endl;
    out << ")"                                                                  << endl;
    out << endl;
}
//--------------------------------------------------------------------------------
// Builds the report table if its a database engine that is supported
bool MainBox::buildTable(QSqlDatabase &db, QTextStream &out, QString &dbServer)
{
    bool result = false;  // be pessimistic
    QString stCommand;
    QString stDriver = db.driverName();

    if(!getSqlDriver(stDriver))
    {
        displayNotSupportedMesg(db, out);
        return result;
    }

    if(stDriver == "QODBC")
    {
        if(!odbcSanityCheck(db, dbServer))
        {
            out << "If -databaseURL uses the ODBC protocol then -dbengine " << endl;
            out << "must indicate the type of database being connected to via " << endl;
            out << "ODBC. Eg. mysql, db2, psql" << endl;
            out << "-dbengine=" << dbServer << " Do not know how to create report table for this engine" << endl;
            displayNotSupportedMesg(db, out);
            return result;
        }
        stDriver = dbServer;
    }

    if(stDriver == "QOCI")
    {
        stCommand = getSqlFromTag("crt01", stDriver);
        execTableBuild(stCommand, out);
        stCommand = getSqlFromTag("crt02", stDriver);
        execTableBuild(stCommand, out);
        stCommand = getSqlFromTag("crt03", stDriver);
        execTableBuild(stCommand, out);
    }
    else
    {
        stCommand = getSqlFromTag("crt01", stDriver);
        execTableBuild(stCommand, out);

        // Create the labeldef table
        stCommand = getSqlFromTag("crt04", stDriver);
        if(execTableBuild(stCommand, out))
        {
            stCommand = getSqlFromTag("crt07", stDriver);
            execTableBuild(stCommand, out);
        }
    }

    return true;
}
//--------------------------------------------------------------------------------
void MainBox::create_report_tables(void)
{
    QTextStream out(stdout, QIODevice::WriteOnly);
    QString dbServer = "";
    drop_table("report");
    drop_table("labeldef");
    qDebug() << buildTable(db, out, dbServer);

    /*
    "CREATE TABLE report ( "
    " report_id SERIAL PRIMARY KEY, "
    " report_name TEXT, "
    " report_descrip TEXT NOT NULL, "
    " report_grade INTEGER NOT NULL DEFAULT 0, "
    " report_source TEXT) " }
    */
    QSqlQuery sql;
    sql.prepare("INSERT INTO report (report_id, report_name, report_descrip, report_grade, report_source) "
                "VALUES (:report_id, :report_name, :report_descrip, :report_grade, :report_source)");
    sql.bindValue(":report_id", 0);
    sql.bindValue(":report_name", "my_report");
    sql.bindValue(":report_descrip", "my_descr");
    sql.bindValue(":report_grade", 0);
    sql.bindValue(":report_source", "yyy");
    qDebug() << "sql.exec()" << sql.exec();

    /*
    "CREATE TABLE labeldef ( "
    " labeldef_id serial PRIMARY KEY, "
    " labeldef_name text NOT NULL, "
    " labeldef_papersize text NOT NULL, "
    " labeldef_columns integer NOT NULL, "
    " labeldef_rows integer NOT NULL, "
    " labeldef_width integer NOT NULL, "
    " labeldef_height integer NOT NULL, "
    " labeldef_start_offset_x integer NOT NULL, "
    " labeldef_start_offset_y integer NOT NULL, "
    " labeldef_horizontal_gap integer NOT NULL, "
    " labeldef_vertical_gap integer NOT NULL ) "}
    */
    sql.prepare("INSERT INTO labeldef (labeldef_id, labeldef_name, labeldef_papersize, labeldef_columns, labeldef_rows, labeldef_width, labeldef_height, labeldef_start_offset_x, labeldef_start_offset_y, labeldef_horizontal_gap, labeldef_vertical_gap ) "
                "VALUES (:labeldef_id, :labeldef_name, :labeldef_papersize, :labeldef_columns, :labeldef_rows, :labeldef_width, :labeldef_height, :labeldef_start_offset_x, :labeldef_start_offset_y, :labeldef_horizontal_gap, :labeldef_vertical_gap)");
    sql.bindValue(":labeldef_id", 0);
    sql.bindValue(":labeldef_name", "label_1");
    sql.bindValue(":labeldef_papersize", "A4");
    sql.bindValue(":labeldef_columns", 10);
    sql.bindValue(":labeldef_rows", 10);
    sql.bindValue(":labeldef_width", 100);
    sql.bindValue(":labeldef_height", 200);
    sql.bindValue(":labeldef_start_offset_x", 10);
    sql.bindValue(":labeldef_start_offset_y", 10);
    sql.bindValue(":labeldef_horizontal_gap", 10);
    sql.bindValue(":labeldef_vertical_gap", 10);
    qDebug() << "sql.exec()" << sql.exec();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    int err = open_database("QSQLITE5", "./rus-tat.dict");
    qDebug() << "open_database return" << err;
    if(err)
        return;

    QSqlQuery dict_sql;
    dict_sql.prepare("select * from sqlite_master where type = 'table'");
    qDebug() << dict_sql.exec();


    return;


    bool ok;

    QString temp =
            "<!DOCTYPE openRPTDef>"
            "<report>"
            "   <title>b</title>"
            "   <name>a</name>"
            "   <description>c</description>"
            "   <grid>"
            "       <x>0.05</x>"
            "       <y>0.05</y>"
            "   </grid>"
            "   <size>A4</size>"
            "   <portrait/>"
            "   <topmargin>100</topmargin>"
            "   <bottommargin>100</bottommargin>"
            "   <rightmargin>100</rightmargin>"
            "   <leftmargin>100</leftmargin>"
            "   <querysource>"
            "       <name>my_query</name>"
            "       <sql>SELECT * FROM PERSON</sql>"
            "   </querysource>"
            "   <section>"
            "       <name>my_section</name>"
            "       <group>"
            "           <name>my_group</name>"
            "           <column></column>"
            "       </group>"
            "   <detail>"
            "   <key>"
            "       <query>my_query</query>"
            "   </key>"
            "   <height>898</height>"
            "   <line>"
            "       <xstart>61</xstart>"
            "       <ystart>91</ystart>"
            "       <xend>161</xend>"
            "       <yend>91</yend>"
            "       <weight>0</weight>"
            "   </line>"
            "   <field>"
            "   <rect>"
            "       <x>206</x>"
            "       <y>84</y>"
            "       <width>100</width>"
            "       <height>25</height>"
            "   </rect>"
            "   <font>"
            "   <face>Helvetica</face>"
            "       <size>12</size>"
            "       <weight>normal</weight>"
            "   </font>"
            "   <left/>"
            "   <top/>"
            "   <data>"
            "       <query>my_query</query>"
            "       <column>name</column>"
            "   </data>"
            "   </field>"
            "   </detail>"
            "   </section>"
            "</report>";

    //QPrinter *printer = new QPrinter(QPrinter::HighResolution);

    orReport f_report("my first report", db);
    //    qDebug() << report.print(printer);
    if(f_report.isValid())
    {
        emit info("report is OK");
        //qDebug() << "print" << f_report.print();
        qDebug() << "exportToPDF" << f_report.exportToPDF("xxx.pdf");
    }
    else
    {
        emit error("report is FAILED");
        f_report.reportError(this);
    }
    return;

    qDebug() << "test()";

    //return;

    //    orReport report(db);
    //    qDebug() << "print" << report.print();
    //    qDebug() << "exportToPDF" << report.exportToPDF("xxx.pdf");

    //    view_table("person");

    //    QPrinter *printer = new QPrinter(QPrinter::HighResolution);

    //ParameterList params;
    //params.append("age", 30);
    //    orReport report("person", params, db);
    orReport report("my_report", db);
    //    qDebug() << report.print(printer);
    if(report.isValid())
        emit info("report is OK");
    else
    {
        emit error("report is FAILED");
        report.reportError(this);
    }

    //    qDebug() << "exportToPDF" << report.exportToPDF("xxx.pdf");
    return;

    QSqlQuery sql;
    sql.prepare("create table person ("
                "id integer primary key, "
                "firstname varchar(20), "
                "lastname varchar(30), "
                "age integer)");
    ok = sql.exec();
    if(!ok)
    {
        emit error(QString("query [%1] not exec!").arg(sql.lastQuery()));
        return;
    }

    sql.prepare("create table test_table "
                "(id integer primary key)");
    ok = sql.exec();
    if(!ok)
    {
        emit error(QString("query [%1] not exec!").arg(sql.lastQuery()));
        return;
    }

    sql.prepare("INSERT INTO person (id, firstname, lastname, age) "
                "VALUES (:id, :firstname, :lastname, :age)");
    for(int i=0; i<10; i++)
    {
        sql.bindValue(":id", i);
        sql.bindValue(":age", 30);
        sql.bindValue(":firstname", "Иван");
        sql.bindValue(":lastname",  "Петров");
        sql.exec();
    }

    QMapIterator<QString, QVariant> i(sql.boundValues());
    while (i.hasNext())
    {
        i.next();
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        qDebug() << i.key().toAscii().data()
                 << i.value().toString().toAscii().data();
#else
        qDebug() << i.key().toLocal8Bit().data()
                 << i.value().toString().toLocal8Bit().data();
#endif
    }

    view_table("person");

    //drop_table("person");
    //drop_table("test_table");

    close_database("customdb");
    emit debug("тест");
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
