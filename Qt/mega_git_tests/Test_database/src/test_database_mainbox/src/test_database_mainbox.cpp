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
#include "ui_test_database_mainbox.h"
//--------------------------------------------------------------------------------
#include "test_database_mainbox.hpp"
#include "defines.hpp"
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
    init();;
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    if(model)
    {
        delete model;
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    model = new QSqlQueryModel;

    connect(ui->btn_open_database,          &QPushButton::clicked,  this,   &MainBox::f_open_database);
    connect(ui->btn_close_database,         &QPushButton::clicked,  this,   &MainBox::f_close_database);

    connect(ui->btn_create_all_tables,      &QPushButton::clicked,  this,   &MainBox::f_create_all_tables);
    connect(ui->btn_create_inventory_table, &QPushButton::clicked,  this,   &MainBox::f_create_inventory_table);
    connect(ui->btn_create_item_table,      &QPushButton::clicked,  this,   &MainBox::f_create_item_table);
    connect(ui->btn_drop_all_tables,        &QPushButton::clicked,  this,   &MainBox::f_drop_all_tables);
    connect(ui->btn_drop_inventory_table,   &QPushButton::clicked,  this,   &MainBox::f_drop_inventory_table);
    connect(ui->btn_drop_item_table,        &QPushButton::clicked,  this,   &MainBox::f_drop_item_table);
    connect(ui->btn_view_inventory_table,   &QPushButton::clicked,  this,   &MainBox::f_view_inventory_table);
    connect(ui->btn_view_item_table,        &QPushButton::clicked,  this,   &MainBox::f_view_item_table);
    connect(ui->btn_append_data_inventory,  &QPushButton::clicked,  this,   &MainBox::f_append_data_inventory);
    connect(ui->btn_append_data_item,       &QPushButton::clicked,  this,   &MainBox::f_append_data_item);
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
    commands.append({ id++, "test",  &MainBox::test });

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
bool MainBox::f_open_database(void)
{
    db = new Database("QSQLITE", "test.db");
    //db = new Database("QPSQL", "test.db", this);
    return db->open();
}
//--------------------------------------------------------------------------------
bool MainBox::f_close_database(void)
{
    db->close();
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test(void)
{
    emit info("Test");
    block_interface(true);

    QString query;
    for(int y=0; y<5; y++)
    {
        for(int x=0; x<5; x++)
        {
            QString name = QString("name %1 %2")
                    .arg(x)
                    .arg(y);
            query = QString("UPDATE inventory SET name = '%1' WHERE pos_x = %2 AND pos_y = %3;")
                    .arg(name)
                    .arg(x)
                    .arg(y);
            emit info(query);

            model->setQuery(query);
        }
    }
    ui->tv_table->setModel((model));

    f_view_inventory_table();
    block_interface(false);
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::f_create_all_tables(void)
{
    emit trace(Q_FUNC_INFO);
    block_interface(true);

    bool ok;
    ok = f_create_inventory_table();
    if(!ok)
    {
        block_interface(false);
        return ok;
    }
    ok = f_create_item_table();
    if(!ok)
    {
        block_interface(false);
        return ok;
    }

    block_interface(false);
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::f_create_inventory_table(void)
{
    emit trace(Q_FUNC_INFO);
    block_interface(true);

    int res = messagebox_question("Create", "Вы уверены, что хотите создать таблицу inventory");
    if(res != QMessageBox::Yes)
    {
        block_interface(false);
        return false;
    }

    emit info("Create inventory");

    QSqlQuery sql;
    QString name = "inventory";
    bool ok = sql.exec(QString("CREATE TABLE %1 (pos_x INTEGER, pos_y INTEGER, name TEXT)").arg(name));
    if(!ok)
    {
        block_interface(false);
        emit error(QString("table (%1) not created! ERROR: %2")
                   .arg(name)
                   .arg(sql.lastError().text()));
    }

    block_interface(false);
    return ok;
}
//--------------------------------------------------------------------------------
bool MainBox::f_create_item_table(void)
{
    emit trace(Q_FUNC_INFO);
    block_interface(true);

    int res = messagebox_question("Create", "Вы уверены, что хотите создать таблицу item");
    if(res != QMessageBox::Yes)
    {
        block_interface(false);
        return false;
    }

    emit info("Create item");

    QSqlQuery sql;
    QString name = "item";
    bool ok = sql.exec(QString("CREATE TABLE %1 (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, path TEXT);").arg(name));
    if(!ok)
    {
        block_interface(false);
        emit error(QString("table (%1) not created! ERROR: %2")
                   .arg(name)
                   .arg(sql.lastError().text()));
    }

    block_interface(false);
    return ok;
}
//--------------------------------------------------------------------------------
bool MainBox::f_drop_all_tables(void)
{
    emit trace(Q_FUNC_INFO);

    f_drop_inventory_table();
    f_drop_item_table();
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::f_drop_inventory_table(void)
{
    block_interface(true);

    int res = messagebox_question("Drop", "Вы уверены, что хотите сбросить таблицу inventory");
    if(res != QMessageBox::Yes)
    {
        block_interface(false);
        return false;
    }

    emit trace(Q_FUNC_INFO);
    emit info("Drop inventory");
    QString name = "inventory";
    bool ok = db->drop_table(name);
    if(!ok)
    {
        block_interface(false);
        emit error(QString("table (%1) not drop!").arg(name));
    }
    emit info("OK");

    block_interface(false);
    return ok;
}
//--------------------------------------------------------------------------------
bool MainBox::f_drop_item_table(void)
{
    block_interface(true);

    int res = messagebox_question("Drop", "Вы уверены, что хотите сбросить таблицу item");
    if(res != QMessageBox::Yes)
    {
        block_interface(false);
        return false;
    }

    emit trace(Q_FUNC_INFO);
    emit info("Drop item");
    QString name = "item";
    bool ok = db->drop_table(name);
    if(!ok)
    {
        block_interface(false);
        emit error(QString("table (%1) not drop!").arg(name));
    }
    emit info("OK");

    block_interface(false);
    return ok;
}
//--------------------------------------------------------------------------------
bool MainBox::f_view_inventory_table(void)
{
    emit trace(Q_FUNC_INFO);
    block_interface(true);

    QString query = "SELECT * FROM inventory";
    emit info(query);

    model->setQuery(query);
    ui->tv_table->setModel(model);

    block_interface(false);
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::f_view_item_table(void)
{
    emit trace(Q_FUNC_INFO);
    block_interface(true);

    QString query = "SELECT * FROM item";
    emit info(query);

    model->setQuery(query);
    ui->tv_table->setModel((model));

    block_interface(false);
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::f_append_data_inventory(void)
{
    QSqlQuery query;
    block_interface(true);

    for(int y=0; y<5; y++)
    {
        for(int x=0; x<5; x++)
        {
            query.prepare("INSERT INTO inventory (pos_x, pos_y, name) VALUES (:pos_x, :pos_y, :name)");
            query.bindValue(":pos_x", x);
            query.bindValue(":pos_y", y);
            query.bindValue(":name", "test");
            emit info(query.executedQuery());
            bool ok = query.exec();
            if(!ok)
            {
                emit error(QString("query [%1] not exec! ERROR: %2")
                           .arg(query.lastQuery())
                           .arg(query.lastError().text()));
                block_interface(false);
                return false;
            }
        }
    }

    block_interface(false);
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::f_append_data_item(void)
{
    QSqlQuery query;
    block_interface(true);

    for(int n=0; n<5; n++)
    {
        query.prepare("INSERT INTO item (path) VALUES (:path)");
        query.bindValue(":path", QString("test %1").arg(n));
        emit info(query.executedQuery());
        bool ok = query.exec();
        if(!ok)
        {
            emit error(QString("query [%1] not exec! ERROR: %2")
                       .arg(query.lastQuery())
                       .arg(query.lastError().text()));
            block_interface(false);
            return false;
        }
    }

    block_interface(false);
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
