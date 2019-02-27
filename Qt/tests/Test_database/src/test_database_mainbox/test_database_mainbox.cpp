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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
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
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar(tr("testbar"));
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    cb_test->addItem(TEST_0_TEXT, QVariant(Qt::UserRole + TEST_0));
    cb_test->addItem(TEST_1_TEXT, QVariant(Qt::UserRole + TEST_1));
    cb_test->addItem(TEST_2_TEXT, QVariant(Qt::UserRole + TEST_2));
    cb_test->addItem(TEST_3_TEXT, QVariant(Qt::UserRole + TEST_3));

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
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;
    switch (cmd)
    {
    case TEST_0: test_0(); break;
    case TEST_1: test_1(); break;
    case TEST_2: test_2(); break;
    case TEST_3: test_3(); break;
    default: break;
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
void MainBox::test_0(void)
{
    emit info("Test_0");

    bool ok = false;
    emit info("open database");
    ok = open_database();
    if(!ok)
    {
        emit error("db not open!");
        return;
    }

    drop_tables();
    create_tables();

    emit info("insert film");
    for(int n=0; n<5; n++)
    {
        QCoreApplication::processEvents();
        ok = insert_film(QString("name %1").arg(n),
                         QString("comment %1").arg(n));
        if(!ok) return;
    }

    emit info("close database");
    close_database();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void MainBox::test_1(void)
{
    bool ok = false;
    ok = open_database();
    if(!ok)
    {
        emit error("db not open!");
        return;
    }
    db->view("SELECT * FROM FILM");
    close_database();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void MainBox::test_2(void)
{
    bool ok = false;
    ok = open_database();
    if(!ok)
    {
        emit error("db not open!");
        return;
    }
    QSqlQuery sql;
    ok = sql.exec("DELETE FROM FILM WHERE name!='name 1'");
    if(!ok)
    {
        emit error("(film) deleted!");
        emit error(sql.lastError().text());
    }
    close_database();
    emit info("OK");
}
//--------------------------------------------------------------------------------
void MainBox::test_3(void)
{
    emit error("не готово!");
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
