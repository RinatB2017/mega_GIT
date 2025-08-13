/*********************************************************************************
**                                                                              **
**     Copyright (C) 2025                                                       **
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
#include "test_LimeReport_mainbox.hpp"
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
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    create_test_bar();
#endif
    create_programm_bar();
    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::create_test_bar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    if(mw)
    {
        test_commands.clear(); int id = 0;
        test_commands.append({ id++,    "create_database",  &MainBox::create_database });
        test_commands.append({ id++,    "show_report",      &MainBox::show_report });
        test_commands.append({ id++,    "test",             &MainBox::test });

        QToolBar *testbar = new QToolBar("testbar");
        Q_ASSERT(testbar);
        testbar->setObjectName("testbar");
        mw->addToolBar(Qt::TopToolBarArea, testbar);

        cb_test = new QComboBox(this);
        Q_ASSERT(cb_test);
        cb_test->setProperty(NO_SAVE, true);
        cb_test->setObjectName("cb_test");
        foreach (CMD command, test_commands)
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

        connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));
    }
    else
    {
        emit error("mw not found!");
    }
}
//--------------------------------------------------------------------------------
void MainBox::create_programm_bar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    if(mw)
    {
        programm_commands.clear(); int id = 0;
        programm_commands.append({ id++,    "show_report",  &MainBox::show_report });

        programm_bar = new QToolBar("programm_bar");
        programm_bar->setObjectName("programm_bar");
        mw->addToolBar(Qt::TopToolBarArea, programm_bar);

        cb_programm = new QComboBox(this);
        cb_programm->setObjectName("cb_programm");
        cb_programm->setProperty(NO_SAVE, true);
        foreach (CMD command, programm_commands)
        {
            cb_programm->addItem(command.cmd_text, QVariant(command.cmd));
        }

        programm_bar->addWidget(cb_programm);
        QToolButton *btn_choice_programm = add_button(programm_bar,
                                                      new QToolButton(this),
                                                      qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                                      "choice_programm",
                                                      "choice_programm");
        btn_choice_programm->setObjectName("btn_choice_programm");

        connect(btn_choice_programm,    &QPushButton::clicked,  this,   &MainBox::choice_programm);
    }
    else
    {
        emit error("mw not found!");
    }
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok) return;

    auto cmd_it = std::find_if(
        test_commands.begin(),
        test_commands.end(),
        [cmd](CMD command){ return command.cmd == cmd; }
        );
    if (cmd_it != test_commands.end())
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
void MainBox::choice_programm(void)
{
    bool ok = false;
    int cmd = cb_programm->itemData(cb_programm->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok) return;

    auto cmd_it = std::find_if(
        programm_commands.begin(),
        programm_commands.end(),
        [cmd](CMD command){ return command.cmd == cmd; }
        );
    if (cmd_it != programm_commands.end())
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
bool MainBox::create_database(void)
{
    bool ok;
    QString database_name = "test_database.db";
    QString table_name = "test_table";
    ok = open_database("QSQLITE", database_name);
    if(!ok)
    {
        emit error("DB not open!");
        return false;
    }

    if(check_table_exist(table_name))
    {
        ok = drop_table(table_name);
        if(!ok)
        {
            emit error("drop_table failed!");
            return false;
        }
    }

    ok = create_data_table(table_name);
    if(!ok)
    {
        emit error("create_data_table failed!");
        return false;
    }

    ok = add_data_to_table(table_name, "0 поле_0", "0 поле_1", "0 поле_2");
    if(!ok)
    {
        emit error("add_data_to_table failed!");
        return false;
    }
    ok = add_data_to_table(table_name, "1 поле_0", "1 поле_1", "1 поле_2");
    if(!ok)
    {
        emit error("add_data_to_table failed!");
        return false;
    }
    ok = add_data_to_table(table_name, "2 поле_0", "2 поле_1", "2 поле_2");
    if(!ok)
    {
        emit error("add_data_to_table failed!");
        return false;
    }
    ok = add_data_to_table(table_name, "3 поле_0", "3 поле_1", "3 поле_2");
    if(!ok)
    {
        emit error("add_data_to_table failed!");
        return false;
    }

    close_database("test_database.db");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::show_report(void)
{
#if 1
    report = new LimeReport::ReportEngine(this);
    report->loadFromFile("test_report.lrxml");
    report->previewReport();
#endif

#if 0
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("test_database.db");
    if (!db.open())
    {
        emit error("обработка ошибки подключения");
        return false;
    }

    QSqlQueryModel* model = new QSqlQueryModel(this);
    model->setQuery("SELECT * FROM test_table");

    report = new LimeReport::ReportEngine(this);
    report->dataManager()->addModel("main", model, true);
    report->loadFromFile(":/data/test_report.lrxml");
    report->previewReport();
#endif

#if 0
    QStringList simpleData;
    simpleData << "Значение 1" << "Значение 2" << "Значение 3";

    QStringListModel *stringListModel = new QStringListModel();
    stringListModel->setStringList(simpleData);

    report = new LimeReport::ReportEngine(this);
    report->dataManager()->addModel("string_list", stringListModel, true);
    report->loadFromFile(":/data/test_report.lrxml");
    report->previewReport();
#endif

#if 0
    QStringList simpleData;
    simpleData << "1" << "2" << "3";

    QStringListModel* stringListModel = new QStringListModel();
    stringListModel->setStringList(simpleData);

    report = new LimeReport::ReportEngine(this);
    report->dataManager()->addModel("string_list", stringListModel,true);
    report->dataManager()->setReportVariable("var1", "value 1");
    report->dataManager()->setReportVariable("var2", "value 2");
    report->dataManager()->setReportVariable("var3", "value 3");
    report->loadFromFile(":/data/test_report.lrxml");
    report->previewReport();
#endif

#if 0
    MyFileDialog *dlg = new MyFileDialog("report");
    dlg->setNameFilter("LRXML files (*.lrxml)");
    dlg->setDefaultSuffix("lrxml");
    dlg->setOption(MyFileDialog::DontUseNativeDialog, false);
    dlg->setDirectory(".");
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        QString filename = files.at(0);
        // QStringListModel* stringListModel = new QStringListModel();

        report = new LimeReport::ReportEngine(this);
        // report->dataManager()->addModel("string_list", stringListModel,true);
        report->loadFromFile(filename);
        // report->loadFromFile("demo_reports/simple_list.lrxml");
        report->dataManager()->setReportVariable("TextItem5", QVariant::fromValue(1));
        report->dataManager()->setReportVariable("TextItem6", QVariant("2"));
        report->dataManager()->setReportVariable("TextItem7", QVariant("3"));
        report->previewReport();
        // report->printReport();
    }
    delete dlg;
#endif

    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test(void)
{
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::open_database(const QString &driver_name,
                            const QString &database_name)
{
    emit info(QString("Open driver %1, database_name %2")
                  .arg(driver_name)
                  .arg(database_name));

    this->driver_name = driver_name;
    this->database_name = database_name;

    if(db.databaseName() != database_name)
    {
        if(QSqlDatabase::contains(QSqlDatabase::defaultConnection))
        {
            db = QSqlDatabase::database();
        }
        else
        {
            db = QSqlDatabase::addDatabase(driver_name);
        }
        db.setDatabaseName(database_name);
    }

    bool ok = db.open();
    if(!ok)
    {
        emit error(QString("db %1 not open!").arg(database_name));
        emit error(db.lastError().text());
        return false;
    }
    emit info("Открытие БД прошло успешно");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::create_data_table(const QString &table_name)
{
    QSqlQuery query;
    bool ok;

    query.prepare(QString("CREATE TABLE %1 ("
                          "text1 TEXT, "
                          "text2 TEXT, "
                          "text3 TEXT) ")
                      .arg(table_name));
    ok = query.exec();
    if(ok == false)
    {
        emit error(QString(tr("table '%1' not created"))
                       .arg(table_name));
        return false;
    }
    emit info(QString("Создание таблицы %1 прошло успешно").arg(table_name));
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::add_data_to_table(const QString &table_name,
                                const QString &text1,
                                const QString &text2,
                                const QString &text3)
{
    QSqlQuery query;
    bool ok;

    query.clear();
    query.prepare(QString("INSERT INTO %1 ("
                          "text1, "
                          "text2, "
                          "text3) "
                          "VALUES ("
                          ":text1, "
                          ":text2, "
                          ":text3) ")
                      .arg(table_name));
    query.bindValue(":text1", text1);
    query.bindValue(":text2", text2);
    query.bindValue(":text3", text3);
    ok = query.exec();
    if(!ok)
    {
        emit error(query.lastError().text());
        return false;
    }
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::check_table_exist(const QString &table_name)
{
    QSqlQuery query;

    emit debug(QString("table_name: %1").arg(table_name));
    QString sql = QString("SELECT 1 FROM '%1' LIMIT 1;").arg(table_name);
    return query.exec(sql);
}
//--------------------------------------------------------------------------------
bool MainBox::drop_table(const QString &table_name)
{
    QSqlQuery sql;
    return sql.exec(QString("DROP TABLE %1").arg(table_name));
}
//--------------------------------------------------------------------------------
void MainBox::close_database(const QString &database_name)
{
    if(!db.isValid()) return;
    db.close();
    db.removeDatabase(database_name);
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
