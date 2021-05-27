/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#include <QSqlQuery>
#include <QSqlError>
//--------------------------------------------------------------------------------
#include "ui_test_database_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_database_mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "database.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
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
    setVisible(false);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw);

    QToolBar *testbar = new QToolBar(tr("testbar"));
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    commands.clear(); int id = 0;
    commands.append({ id++, "insert_data",  &MainBox::insert_data });
    commands.append({ id++, "test_sql",     &MainBox::test_sql });
    commands.append({ id++, "test_sql2",    &MainBox::test_sql2 });

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    cb_test->setProperty(NO_SAVE, true);
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok) return;

    auto cmd_it = std::find_if(
                commands.begin(),
                commands.end(),
                [cmd](CMD command){ return command.cmd == cmd; }
            );
    if (cmd_it != commands.end())
    {
        typedef bool (MainBox::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (this->*x)();
        }
        else
        {
            emit error("no func");
        }
    }
}
//--------------------------------------------------------------------------------
bool MainBox::open_database(void)
{
    db = new Database("QSQLITE", "test.db");
    //db = new Database("QPSQL", "test.db", this);
    return db->open();
}
//--------------------------------------------------------------------------------
bool MainBox::create_table_main(void)
{
    QSqlQuery sql;
    bool ok = sql.exec("CREATE TABLE main (folder_ID INTEGER, film_ID INTEGER, comment TEXT)");
    if(!ok)
    {
        emit error("table (main) not created!");
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool MainBox::create_table_folder(void)
{
    QSqlQuery sql;
    bool ok = sql.exec("CREATE TABLE folder (parent_ID INTEGER, ID INTEGER, name TEXT)");
    if(!ok)
    {
        emit error("table (folder) not created!");
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool MainBox::create_table_film(void)
{
    QSqlQuery sql;
    bool ok = sql.exec("CREATE TABLE film (ID INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, comment TEXT)");
    if(!ok)
    {
        emit error("table (film) not created!");
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool MainBox::drop_table_main(void)
{
    bool ok = db->drop_table("main");
    if(!ok)
    {
        emit error("table (main) not drop!");
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool MainBox::drop_table_folder(void)
{
    bool ok = db->drop_table("folder");
    if(!ok)
    {
        emit error("table (folder) not drop!");
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool MainBox::drop_table_film(void)
{
    bool ok = db->drop_table("film");
    if(!ok)
    {
        emit error("table (film) not drop!");
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool MainBox::close_database(void)
{
    db->close();
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::insert_film(QString film_name, QString comment)
{
    QSqlQuery query;
    query.prepare("INSERT INTO film (name, comment) VALUES (:name, :comment)");
    query.bindValue(":name", film_name);
    query.bindValue(":comment", comment);
    bool ok = query.exec();
    if(!ok)
    {
        emit error(QString("query [%1] not exec!").arg(query.lastQuery()));
    }
    return ok;
}
//--------------------------------------------------------------------------------
void MainBox::create_tables(void)
{
    bool ok = false;
    ok = create_table_film();
    if(!ok) return;
    ok = create_table_folder();
    if(!ok) return;
    ok = create_table_main();
    if(!ok) return;
}
//--------------------------------------------------------------------------------
void MainBox::drop_tables()
{
    drop_table_film();
    drop_table_folder();
    drop_table_main();
}
//--------------------------------------------------------------------------------
bool MainBox::insert_data(void)
{
    emit info("insert_data");

    bool ok = false;
    emit info("open database");
    ok = open_database();
    if(!ok)
    {
        emit error("db not open!");
        return false;
    }

    drop_tables();
    create_tables();

    emit info("insert film");
    for(int n=0; n<5; n++)
    {
        QCoreApplication::processEvents();
        ok = insert_film(QString("name %1").arg(n),
                         QString("comment %1").arg(n));
        if(!ok)
        {
            return false;
        }
    }

    emit info("close database");
    close_database();
    emit info("OK");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_sql(void)
{
    bool ok = false;
    ok = open_database();
    if(!ok)
    {
        emit error("db not open!");
        return false;
    }
    db->view("SELECT * FROM FILM");
    close_database();
    emit info("OK");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_sql2(void)
{
    bool ok = false;
    ok = open_database();
    if(!ok)
    {
        emit error("db not open!");
        return false;
    }
    QSqlQuery sql;
    ok = sql.exec("DELETE FROM FILM WHERE name!='name 1'");
    if(!ok)
    {
        emit error("(film) deleted!");
        emit error(sql.lastError().text());
        return false;
    }
    close_database();
    emit info("OK");

    return true;
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
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
