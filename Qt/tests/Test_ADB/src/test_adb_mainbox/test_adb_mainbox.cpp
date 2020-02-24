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
#include "ui_test_adb_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_adb_mainbox.hpp"
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

    if(myProcess)
    {
        myProcess->disconnect();
        myProcess->deleteLater();
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

    myProcess = new QProcess(this);
    connect(myProcess,  SIGNAL(started()),                      this,   SLOT(started()));
    connect(myProcess,  SIGNAL(finished(int)),                  this,   SLOT(finished(int)));
    connect(myProcess,  SIGNAL(error(QProcess::ProcessError)),  this,   SLOT(process_error(QProcess::ProcessError)));
    connect(myProcess,  SIGNAL(readyReadStandardOutput()),      this,   SLOT(readData()));
    connect(myProcess,  SIGNAL(readyReadStandardError()),       this,   SLOT(readData()));

    connect(ui->btn_devices,            &QPushButton::clicked,  this,   &MainBox::f_devices);
    connect(ui->btn_create_screenshot,  &QPushButton::clicked,  this,   &MainBox::f_create_screenshot);
    connect(ui->btn_tap,                &QPushButton::clicked,  this,   &MainBox::f_screen_tap);

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &MainBox::test_0 });
    commands.append({ ID_TEST_1, "test 1", &MainBox::test_1 });
    commands.append({ ID_TEST_2, "test 2", &MainBox::test_2 });
    commands.append({ ID_TEST_3, "test 3", &MainBox::test_3 });
    commands.append({ ID_TEST_4, "test 4", &MainBox::test_4 });
    commands.append({ ID_TEST_5, "test 5", &MainBox::test_5 });
    commands.append({ ID_TEST_6, "test 6", nullptr });

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
void MainBox::run_program(const QString program,
                          const QStringList arguments)
{
    if(program.isEmpty())
    {
        emit error("program is empty");
        return;
    }

    myProcess->start(program, arguments);
}
//--------------------------------------------------------------------------------
void MainBox::readData(void)
{
    QString output = myProcess->readAllStandardOutput();
    QStringList lines = output.split("\n");
    emit debug(QString("received %1 bytes").arg(output.size()));
    emit debug(QString("lines.size() %1").arg(lines.size()));
    for(int n=0; n<lines.size(); n++)
    {
        QString line = lines.at(n);
        emit info(QString("%1").arg(line));
    }
}
//--------------------------------------------------------------------------------
void MainBox::started(void)
{
    emit info("Процесс начат!");
}
//--------------------------------------------------------------------------------
void MainBox::finished(int result)
{
    process_result = result;
    f_busy = false;

    if(process_result == 0)
    {
        emit info("Процесс завершен!");
    }
    else
    {
        emit error(QString("Процесс завершён с ошибкой (%1)").arg(result));
    }
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
bool MainBox::f_devices(void)
{
    QString program = "adb";
    QStringList arguments;

    arguments << "devices";
    run_program(program, arguments);
    while(f_busy)
    {
        QCoreApplication::processEvents();
    }
    return (process_result == 0);
}
//--------------------------------------------------------------------------------
void MainBox::f_create_screenshot(void)
{
    QString program = "adb";
    QStringList arguments;

    if(!f_get_screeshot())      return;
    if(!f_get_file_screeshot()) return;
    f_show_screeshot();
}
//--------------------------------------------------------------------------------
bool MainBox::f_get_screeshot(void)
{
    QString program = "adb";
    QStringList arguments;

    arguments << "shell";
    arguments << "screencap";
    arguments << "-p";
    arguments << "/sdcard/screencap.png";

    f_busy = true;
    run_program(program, arguments);
    while(f_busy)
    {
        QCoreApplication::processEvents();
    }
    return (process_result == 0);
}
//--------------------------------------------------------------------------------
bool MainBox::f_get_file_screeshot(void)
{
    QString program = "adb";
    QStringList arguments;

    arguments << "pull";
    arguments << "/sdcard/screencap.png";

    f_busy = true;
    run_program(program, arguments);
    while(f_busy)
    {
        QCoreApplication::processEvents();
    }
    return (process_result == 0);
}
//--------------------------------------------------------------------------------
void MainBox::f_screen_tap(void)
{
    f_tap(100, 200);
}
//--------------------------------------------------------------------------------
bool MainBox::f_tap(uint pos_x, uint pos_y)
{
    //adb shell input tap 100 100

    QString program = "adb";
    QStringList arguments;

    arguments << "shell";
    arguments << "input";
    arguments << "tap";
    arguments << QString("%1").arg(pos_x);
    arguments << QString("%1").arg(pos_y);

    f_busy = true;
    run_program(program, arguments);
    while(f_busy)
    {
        QCoreApplication::processEvents();
    }
    return (process_result == 0);
}
//--------------------------------------------------------------------------------
void MainBox::f_show_screeshot(void)
{
    ui->lbl_screenshot->setPixmap(QPixmap("screencap.png"));
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
