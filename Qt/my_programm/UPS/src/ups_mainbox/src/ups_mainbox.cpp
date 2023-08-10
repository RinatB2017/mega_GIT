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
    if(timer)
    {
        timer->stop();
        delete timer;
    }

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

    init_display_data();
    init_grapher();
    init_timer();

    ui->btn_run_upsdrvctl->setIcon(qApp->style()->standardIcon(QStyle::SP_CommandLink));
    connect(ui->btn_run_upsdrvctl,  &QToolButton::clicked,  this,   &MainBox::run_upsdrvctl);

    QList<QLCDNumber *> allobj = findChildren<QLCDNumber *>();
    foreach (QLCDNumber *obj, allobj)
    {
        obj->setFixedSize(120, 38);
        obj->setProperty(NO_SAVE, true);
        //obj->setDigitCount(14);
    }

    connect(ui->le_ups,     &QLineEdit::returnPressed,  this,   &MainBox::run);
    connect(ui->btn_run,    &QToolButton::clicked,      this,   &MainBox::run);

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::init_display_data(void)
{
    display_data.append({ "battery.charge",             ui->lcd_battery_charge,             0 });
    display_data.append({ "battery.voltage",            ui->lcd_battery_voltage,            1 });
    display_data.append({ "battery.voltage.high",       ui->lcd_battery_voltage_high,       2 });
    display_data.append({ "battery.voltage.low",        ui->lcd_battery_voltage_low,        3 });
    display_data.append({ "battery.voltage.nominal",    ui->lcd_battery_voltage_nominal,    4 });
    display_data.append({ "input.frequency",            ui->lcd_input_frequency,            5 });
    display_data.append({ "input.voltage",              ui->lcd_input_voltage,              6 });
    display_data.append({ "input.voltage.fault",        ui->lcd_input_voltage_fault,        7 });
    display_data.append({ "input.voltage.nominal",      ui->lcd_input_voltage_nominal,      8 });
    display_data.append({ "output.voltage",             ui->lcd_output_voltage,             9 });
    display_data.append({ "ups.temperature",            ui->lcd_ups_temperature,            10 });
}
//--------------------------------------------------------------------------------
void MainBox::init_grapher(void)
{
    foreach (DATA data, display_data)
    {
        data.curve_id = ui->grapher_widget->add_curve(data.data_name);
    }

    ui->grapher_widget->set_visible_btn_Options(false);
    ui->grapher_widget->set_visible_btn_Load(false);
    ui->grapher_widget->set_visible_btn_Save(false);
    ui->grapher_widget->set_visible_btn_Autoscroll(false);
    ui->grapher_widget->set_visible_btn_Statistic(false);
}
//--------------------------------------------------------------------------------
void MainBox::init_timer(void)
{
    timer = new QTimer(this);
    connect(timer,  &QTimer::timeout,   this,   &MainBox::t_update);

    ui->btn_start->setEnabled(true);
    ui->btn_stop->setEnabled(false);

    ui->sb_interval->setRange(1, 3600);

    ui->btn_run->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_start->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_stop->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop));

    connect(ui->btn_start,  &QToolButton::clicked,      this,   &MainBox::t_start);
    connect(ui->btn_stop,   &QToolButton::clicked,      this,   &MainBox::t_stop);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    if(mw)
    {
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
void MainBox::t_start(void)
{
    timer->start(ui->sb_interval->value() * 1000);

    ui->le_ups->setEnabled(false);

    ui->btn_run->setEnabled(false);
    ui->btn_start->setEnabled(false);
    ui->btn_stop->setEnabled(true);
}
//--------------------------------------------------------------------------------
void MainBox::t_stop(void)
{
    timer->stop();

    ui->le_ups->setEnabled(true);

    ui->btn_run->setEnabled(true);
    ui->btn_start->setEnabled(true);
    ui->btn_stop->setEnabled(false);
}
//--------------------------------------------------------------------------------
void MainBox::t_update(void)
{
    run();
}
//--------------------------------------------------------------------------------
void MainBox::started(void)
{
#ifdef QT_DEBUG
    emit debug("Процесс начат!");
#endif
    if(timer->isActive() == false)
    {
        ui->btn_run->setDisabled(true);
        ui->btn_run_upsdrvctl->setDisabled(true);
    }
}
//--------------------------------------------------------------------------------
void MainBox::finished(int result, QProcess::ExitStatus exitStatus)
{
#ifdef QT_DEBUG
    emit debug(tr("Процесс завершен!"));
#endif
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

    if(timer->isActive() == false)
    {
        ui->btn_run->setEnabled(true);
        ui->btn_run_upsdrvctl->setEnabled(true);
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
            QString temp = sl.at(1);
            qreal t_data = temp.toDouble();
            data.lcd_display->display(t_data);

            if(timer->isActive())
            {
                ui->grapher_widget->add_curve_data(data.curve_id, t_data);
            }
        }
    }
}
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
void MainBox::run_upsdrvctl(void)
{
    if(process)
    {
        emit error("process is running");
        return;
    }

    prepare_QProcess();
    QStringList params;
    params << "upsdrvctl";
    params << "start";
    process->start("sudo", params);
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
