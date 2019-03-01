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
#include "ui_mtc_runner_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mtc_runner_mainbox.hpp"
#include "defines.hpp"
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
    save_widgets(APPNAME);
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createRunBar();
    createScriptBar();

    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(mw)
    {
        mw->load_setting();
    }
    load_widgets(APPNAME);
}
//--------------------------------------------------------------------------------
void MainBox::createRunBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_auto_run = new QCheckBox();
    cb_auto_run->setText("auto");
    cb_auto_run->setObjectName("cb_auto_run");

    testbar->addWidget(cb_auto_run);

    btn_run = add_button(testbar,
                         new QToolButton(this),
                         qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                         "run",
                         "run");
    btn_run->setObjectName("btn_run");

    connect(btn_run, SIGNAL(clicked()), this, SLOT(run()));
}
//--------------------------------------------------------------------------------
void MainBox::createScriptBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    btn_script = new QToolButton();
    btn_script->setText("...");
    btn_script->setObjectName("btn_script");

    le_script_filename = new QLineEdit();
    le_script_filename->setReadOnly(true);
    le_script_filename->setObjectName("le_script_filename");

    QToolBar *scriptbar = new QToolBar(tr("scriptbar"));
    scriptbar->setObjectName("scriptbar");
    mw->addToolBar(Qt::TopToolBarArea, scriptbar);

    scriptbar->addWidget(le_script_filename);
    scriptbar->addWidget(btn_script);

    connect(btn_script, SIGNAL(clicked(bool)), this, SLOT(choice_script()));
    connect(cb_auto_run, SIGNAL(toggled(bool)), this, SLOT(auto_run(bool)));
}
//--------------------------------------------------------------------------------
void MainBox::choice_script(void)
{
    QFileDialog dialog(this);
    dialog.setWindowTitle(tr("Выберите скрипт"));
    dialog.setFileMode(QFileDialog::AnyFile);
    int btn = dialog.exec();
    if(btn == QFileDialog::Accepted)
    {
        QStringList fileNames = dialog.selectedFiles();
        le_script_filename->setText(fileNames.at(0));
    }
}
//--------------------------------------------------------------------------------
void MainBox::auto_run(bool state)
{
    btn_run->setDisabled(state);
    if(state)
    {
        QTimer::singleShot(1000, this, SLOT(procfunc()));
    }
}
//--------------------------------------------------------------------------------
void MainBox::run(void)
{
    if(process)
    {
        emit error(tr("Процесс уже запущен!"));
        return;
    }
    procfunc();
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
void MainBox::procfunc(void)
{
    process = new QProcess();
    process->setProcessChannelMode(QProcess::SeparateChannels);
    //process->setReadChannel(QProcess::StandardOutput);

    connect(process, SIGNAL(started()),                 this, SLOT(started()));
    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(read_data()));
    connect(process, SIGNAL(readyReadStandardError()),  this, SLOT(read_error()));
    connect(process, SIGNAL(finished(int)),             this, SLOT(finished(int)));
    connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(process_error(QProcess::ProcessError)));

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    QString fullPath = env.value("PATH")+ ":/home/boss/bin";
    env.insert("PATH", fullPath);
    process->setProcessEnvironment(env);

    if(le_script_filename->text().isEmpty() == false)
    {
        process->start("sh", QStringList() << le_script_filename->text());
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
void MainBox::finished(int result)
{
    emit info(tr("Процесс завершен!"));
    if(result)
    {
        emit error(QString(tr("code %1")).arg(result));
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
