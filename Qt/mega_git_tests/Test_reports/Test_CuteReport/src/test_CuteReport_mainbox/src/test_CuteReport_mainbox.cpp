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
#include "test_CuteReport_mainbox.hpp"
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
        programm_commands.append({ id++,    "test", &MainBox::test });

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

    ok = add_data_to_table(table_name, "a00", "b00", "b00");
    if(!ok)
    {
        emit error("add_data_to_table failed!");
        return false;
    }
    ok = add_data_to_table(table_name, "a01", "b01", "b01");
    if(!ok)
    {
        emit error("add_data_to_table failed!");
        return false;
    }
    ok = add_data_to_table(table_name, "a02", "b02", "b02");
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
    CuteReport::ReportCore *reportCore = new CuteReport::ReportCore(0, 0, false);
    CuteReport::ReportPreview *preview = new CuteReport::ReportPreview();

    preview->setReportCore(reportCore);
    QString reportUrl;
#ifdef Q_OS_WIN
    reportUrl = "file:/C/Program Files (x86)/CuteReport/development/examples/reports/invoice_1.qtrp";
#else
    reportUrl = "file:/usr/share/cutereport/examples/examples/reports/invoice_1.qtrp";
#endif
    CuteReport::ReportInterface * reportObject = reportCore->loadReport(reportUrl);
    if (!reportObject)
    {
        QMessageBox::critical(0, QObject::tr("CuteReport example"),
                              QObject::tr("Report file loading error"),
                              QMessageBox::Ok);
        return false;
    }
    preview->connectReport(reportObject);
    preview->show();
    preview->run();

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
