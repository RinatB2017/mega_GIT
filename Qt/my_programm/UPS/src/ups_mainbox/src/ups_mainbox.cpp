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
#include "ups_mainbox.hpp"
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

    display_data.append({ "battery.charge",             ui->lcd_battery_charge });
    display_data.append({ "battery.voltage",            ui->lcd_battery_voltage });
    display_data.append({ "battery.voltage.high",       ui->lcd_battery_voltage_high });
    display_data.append({ "battery.voltage.low",        ui->lcd_battery_voltage_low });
    display_data.append({ "battery.voltage.nominal",    ui->lcd_battery_voltage_nominal });
    display_data.append({ "input.frequency",            ui->lcd_input_frequency });
    display_data.append({ "input.voltage",              ui->lcd_input_voltage });
    display_data.append({ "input.voltage.fault",        ui->lcd_input_voltage_fault });
    display_data.append({ "input.voltage.nominal",      ui->lcd_input_voltage_nominal });
    display_data.append({ "output.voltage",             ui->lcd_output_voltage });
    display_data.append({ "ups.temperature",            ui->lcd_ups_temperature });

    QList<QLCDNumber *> allobj = findChildren<QLCDNumber *>();
    foreach (QLCDNumber *obj, allobj)
    {
        obj->setFixedSize(200, 38);
        obj->setProperty(NO_SAVE, true);
        //obj->setDigitCount(14);
    }

    connect(ui->le_ups,     &QLineEdit::returnPressed,  this,   &MainBox::run);
    connect(ui->btn_run,    &QToolButton::clicked,      this,   &MainBox::run);

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test",     &MainBox::test });

    testbar = new QToolBar("testbar");
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

#ifndef NO_MENU
    //mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
#endif    
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
void MainBox::prepare_QProcess(void)
{
    process = new QProcess();
    process->setProcessChannelMode(QProcess::SeparateChannels);
    //process->setReadChannel(QProcess::StandardOutput);

    connect(process,    &QProcess::started,                 this,   &MainBox::started);
    connect(process,    &QProcess::readyReadStandardOutput, this,   &MainBox::read_data);
    connect(process,    &QProcess::readyReadStandardError,  this,   &MainBox::read_error);
    connect(process,    static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this,       static_cast<void (MainBox::*)(int, QProcess::ExitStatus)>(&MainBox::finished));
    connect(process,    static_cast<void (QProcess::*)(QProcess::ProcessError)>(&QProcess::errorOccurred),
            this,       static_cast<void (MainBox::*)(QProcess::ProcessError)>(&MainBox::process_error));

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    QString fullPath = env.value("PATH")+ ":/home/boss/bin";
    env.insert("PATH", fullPath);
    process->setProcessEnvironment(env);
}
//--------------------------------------------------------------------------------
void MainBox::started(void)
{
    emit info("Процесс начат!");
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
    process = 0;
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
void MainBox::read_data(void)
{
    QByteArray data = process->readAllStandardOutput();
    //emit info(data);

    QString text = data.data();
    QStringList sl = text.split('\n');
    foreach (QString line, sl)
    {
        show_data(line);
        emit debug(QString("line: %1").arg(line));
    }
}
//--------------------------------------------------------------------------------
void MainBox::show_data(const QString &line)
{
    if(line.isEmpty())
    {
        return;
    }
    QStringList sl = line.split(':');
    if(sl.count() != 2)
    {
        return;
    }
    foreach (DATA data, display_data)
    {
        if(data.data_name == sl.at(0))
        {
            data.display->display(sl.at(1));
        }
    }
}
//--------------------------------------------------------------------------------
#if 0
battery.charge: 100
battery.voltage: 13.60
battery.voltage.high: 13.60
battery.voltage.low: 10.60
battery.voltage.nominal: 12.0
input.frequency: 50.1
input.voltage: 231.3
input.voltage.fault: 231.3
input.voltage.nominal: 220
output.voltage: 231.3
ups.temperature: 25.0
#endif
//--------------------------------------------------------------------------------
void MainBox::read_error(void)
{
    QByteArray data = process->readAllStandardError();
    emit error(data);
}
//--------------------------------------------------------------------------------
void MainBox::run(void)
{
    if(process)
    {
        emit error("process is running");
        return;
    }

    prepare_QProcess();
    //clear_log();
    if(ui->le_ups->text().isEmpty() == false)
    {
        QStringList params;
        params << ui->le_ups->text();
        process->start("upsc", params);
    }
}
//--------------------------------------------------------------------------------
bool MainBox::test(void)
{
    emit info("Test");
    emit info(qApp->applicationVersion());
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
