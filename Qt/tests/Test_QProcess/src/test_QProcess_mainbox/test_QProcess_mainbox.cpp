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
    //createTestBar();

    connect(ui->btn_run_sh,     SIGNAL(clicked(bool)),  this,   SLOT(run_sh()));
    connect(ui->btn_run_gcc,    SIGNAL(clicked(bool)),  this,   SLOT(run_gcc()));
    connect(ui->btn_run_kate,   SIGNAL(clicked(bool)),  this,   SLOT(run_kate()));
    connect(ui->btn_run_command,    SIGNAL(clicked(bool)),  this,   SLOT(run_command()));

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test 0", &MainBox::test_0 });
    commands.append({ id++, "test 1", &MainBox::test_1 });
    commands.append({ id++, "test 2", &MainBox::test_2 });
    commands.append({ id++, "test 3", &MainBox::test_3 });
    commands.append({ id++, "test 4", &MainBox::test_4 });
    commands.append({ id++, "test 5", &MainBox::test_5 });
    commands.append({ id++, "test 6", nullptr });

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

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
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
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef bool (MainBox::*my_mega_function)(void);
            my_mega_function x;
            x = command.func;
            if(x)
            {
                (this->*x)();
            }
            else
            {
                emit error("no func");
            }

            return;
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
        emit error(QString(tr("code %1")).arg(result));
        switch (exitStatus)
        {
        case QProcess::NormalExit:
            emit error("The process exited normally.");
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
        process->start(ui->le_command->text());
    }
}
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit info("Test_0()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_1()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit info("Test_2()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit info("Test_3()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit info("Test_4()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit info("Test_5()");
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
