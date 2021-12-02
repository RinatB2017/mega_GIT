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
#include "ui_test_PostgreSQL_mainbox.h"
//--------------------------------------------------------------------------------
#include "test_PostgreSQL_mainbox.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

    connect(ui->btn_open,   &QPushButton::clicked,
            this,           &MainBox::open);
    connect(ui->btn_close,  &QPushButton::clicked,
            this,           &MainBox::close);
    connect(ui->btn_show,   &QPushButton::clicked,
            this,           &MainBox::show);

    connect(ui->btn_create_table,   &QPushButton::clicked,
            this,                   &MainBox::create_table);
    connect(ui->btn_run_sql,        &QPushButton::clicked,
            this,                   &MainBox::run_sql);

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test",                 &MainBox::test });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

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
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test,    &QToolButton::clicked,  this,   &MainBox::choice_test);
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
    else
    {
        emit error("not found");
    }
}
//--------------------------------------------------------------------------------
bool MainBox::test(void)
{
    fail();

    return true;
}
//--------------------------------------------------------------------------------
void MainBox::open(void)
{
    if(QSqlDatabase::contains(QSqlDatabase::defaultConnection))
    {
        db = QSqlDatabase::database();
    }
    else
    {
        db = QSqlDatabase::addDatabase("QPSQL");
    }
    db.setHostName(ui->le_HostName->text());
    db.setDatabaseName(ui->le_DatabaseName->text());
    db.setUserName(ui->le_UserName->text());
    db.setPassword(ui->le_Password->text());
    bool ok = db.open();

    if(ok == false)
    {
        emit error(QString("db failed:")
                   .arg(db.lastError().text()));
        return;
    }
    emit info("Open: OK");
}
//--------------------------------------------------------------------------------
void MainBox::close(void)
{
    db.close();
    emit info("Close: OK");
}
//--------------------------------------------------------------------------------
void MainBox::show(void)
{
    if(db.isOpen() ==  false)
    {
        emit error("Database not open!");
        return;
    }

    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT * FROM pg_type");
    ui->tableView->setModel(model);
}
//--------------------------------------------------------------------------------
void MainBox::create_table(void)
{
    if(db.isOpen() ==  false)
    {
        emit error("Database not open!");
        return;
    }

    QString query = QString( "CREATE TABLE service_groups (ident text PRIMARY KEY,name text NOT NULL,active boolean);" );
    QSqlQuery tableQuery(db);
    bool ok = tableQuery.exec(query);
    if(ok)
    {
        emit info("YES");
    }
    else
    {
        emit error("NO");
    }
}
//--------------------------------------------------------------------------------
void MainBox::run_sql(void)
{
    if(db.isOpen() ==  false)
    {
        emit error("Database not open!");
        return;
    }

    QString query = ui->te_sql_query->toPlainText();
    QSqlQuery tableQuery(db);
    bool ok = tableQuery.exec(query);
    if(ok)
    {
        emit info("YES");
    }
    else
    {
        emit error("NO");
    }
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
