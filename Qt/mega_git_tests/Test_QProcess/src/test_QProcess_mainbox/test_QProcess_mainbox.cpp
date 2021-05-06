/*********************************************************************************
**                                                                              **
**     Copyright (C) 2019                                                       **
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
#include "ui_test_QProcess_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_QProcess_mainbox.hpp"
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
    createTestBar();
#endif

    connect(ui->btn_run_sh,         SIGNAL(clicked(bool)),  this,   SLOT(run_sh()));
    connect(ui->btn_run_gcc,        SIGNAL(clicked(bool)),  this,   SLOT(run_gcc()));
    connect(ui->btn_run_kate,       SIGNAL(clicked(bool)),  this,   SLOT(run_kate()));
    connect(ui->btn_run_command,    SIGNAL(clicked(bool)),  this,   SLOT(run_command()));

    connect(ui->le_script_filename, &QLineEdit::editingFinished,    this,   &MainBox::run_sh);
    connect(ui->le_gcc_param,       &QLineEdit::editingFinished,    this,   &MainBox::run_gcc);
    connect(ui->le_kate_param,      &QLineEdit::editingFinished,    this,   &MainBox::run_kate);
    connect(ui->le_command,         &QLineEdit::editingFinished,    this,   &MainBox::run_command);
    connect(ui->le_param,           &QLineEdit::editingFinished,    this,   &MainBox::run_command);

    ui->btn_run_sh->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_run_gcc->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_run_kate->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_run_command->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test", &MainBox::test });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
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

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    //mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok)
    {
        return;
    }
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
void MainBox::read_data(void)
{
    QByteArray data = process->readAllStandardOutput();
    emit info(data);
}
//--------------------------------------------------------------------------------
void MainBox::read_error(void)
{
    QByteArray data = process->readAllStandardError();
    emit error(data);
}
//--------------------------------------------------------------------------------
void MainBox::started(void)
{
    emit info(tr("Процесс начат!"));
}
//--------------------------------------------------------------------------------
void MainBox::finished(int result, QProcess::ExitStatus exitStatus)
{
    emit info(tr("Процесс завершен!"));
    if(result)
    {
        emit info(QString(tr("code %1")).arg(result));
        switch (exitStatus)
        {
        case QProcess::NormalExit:
            emit info("The process exited normally.");
            break;
        case QProcess::CrashExit:
            emit error("The process crashed.");
            break;
        }
    }

    delete process;
}
//--------------------------------------------------------------------------------
void MainBox::process_error(QProcess::ProcessError p_error)
{
    switch(p_error)
    {
    case QProcess::FailedToStart:
        emit error("FailedToStart");
        break;

    case QProcess::Crashed:
        emit error("Crashed");
        break;

    case QProcess::Timedout:
        emit error("Timedout");
        break;

    case QProcess::ReadError:
        emit error("ReadError");
        break;

    case QProcess::WriteError:
        emit error("WriteError");
        break;

    case QProcess::UnknownError:
        emit error("UnknownError");
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_QProcess(void)
{
    process = new QProcess();
    process->setProcessChannelMode(QProcess::SeparateChannels);
    //process->setReadChannel(QProcess::StandardOutput);

    connect(process, SIGNAL(started()),                             this,   SLOT(started()));
    connect(process, SIGNAL(readyReadStandardOutput()),             this,   SLOT(read_data()));
    connect(process, SIGNAL(readyReadStandardError()),              this,   SLOT(read_error()));
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)),   this,   SLOT(finished(int, QProcess::ExitStatus)));
    connect(process, SIGNAL(error(QProcess::ProcessError)),         this,   SLOT(process_error(QProcess::ProcessError)));

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    QString fullPath = env.value("PATH")+ ":/home/boss/bin";
    env.insert("PATH", fullPath);
    process->setProcessEnvironment(env);
}
//--------------------------------------------------------------------------------
void MainBox::run_sh(void)
{
    prepare_QProcess();
    if(ui->le_script_filename->text().isEmpty() == false)
    {
        QStringList params = ui->le_script_filename->text().split(" ");
        process->start("sh", params);
    }
}
//--------------------------------------------------------------------------------
void MainBox::run_gcc(void)
{
    prepare_QProcess();
    if(ui->le_gcc_param->text().isEmpty() == false)
    {
        QStringList params = ui->le_gcc_param->text().split(" ");
        process->start("gcc", params);
    }
}
//--------------------------------------------------------------------------------
void MainBox::run_kate(void)
{
    prepare_QProcess();
    if(ui->le_kate_param->text().isEmpty() == false)
    {
        QStringList params = ui->le_kate_param->text().split(" ");
        process->start("kate", params);
    }
}
//--------------------------------------------------------------------------------
void MainBox::run_command(void)
{
    prepare_QProcess();
    if(ui->le_command->text().isEmpty() == false)
    {
        process->start(ui->le_command->text(), QStringList() << ui->le_param->text());
    }
}
//--------------------------------------------------------------------------------
bool MainBox::test(void)
{
    emit info("Test()");
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
