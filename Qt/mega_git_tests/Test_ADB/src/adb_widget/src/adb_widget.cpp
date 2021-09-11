/*********************************************************************************
**                                                                              **
**     Copyright (C) 2021                                                       **
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
#include "adb_widget.hpp"
#include "ui_adb_widget.h"
//--------------------------------------------------------------------------------
ADB_widget::ADB_widget(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::ADB_widget)
{
    init();;
}
//--------------------------------------------------------------------------------
ADB_widget::~ADB_widget()
{
    if(timer_autoshot)
    {
        timer_autoshot->stop();
        disconnect(timer_autoshot,  &QTimer::timeout,   this,   &ADB_widget::f_create_screenshot);

        timer_autoshot->deleteLater();
    }
    if(myProcess)
    {
        disconnect(myProcess,   &QProcess::started,                  this,   &ADB_widget::started);
        disconnect(myProcess,   static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
                   this,        &ADB_widget::finished);
        disconnect(myProcess,   &QProcess::errorOccurred,            this,   &ADB_widget::process_error);
        disconnect(myProcess,   &QProcess::readyReadStandardOutput,  this,   &ADB_widget::readData);
        disconnect(myProcess,   &QProcess::readyReadStandardError,   this,   &ADB_widget::readData);

        myProcess->deleteLater();
    }

    delete ui;
}
//--------------------------------------------------------------------------------
void ADB_widget::init(void)
{
    ui->setupUi(this);

    s_width = 700;
    s_height = 1000;

    ui->sb_pos_x->setRange(0, s_width);
    ui->sb_pos_y->setRange(0, s_height);

    copy_file(":/scrcpy-server", "scrcpy-server");

    create_process();

    //---
    ui->sb_port->setRange(0, 0xFFFF);

    server = new TCP_Server(this);
    connect(server, &TCP_Server::output,
            this,   &ADB_widget::f_get_data);
    connect(ui->btn_start,  &QPushButton::clicked,  this,   &ADB_widget::f_start);
    connect(ui->btn_stop,   &QPushButton::clicked,  this,   &ADB_widget::f_stop);
    //---

    //---
    sl_commands << "KEYCODE_SETTINGS"
                << "KEYCODE_CAMERA"
                << "KEYCODE_POWER"
                << "KEYCODE_ZOOM_OUT"
                << "KEYCODE_ZOOM_IN"
                << "KEYCODE_VOLUME_UP"
                << "KEYCODE_VOLUME_DOWN"
                << "KEYCODE_VOLUME_MUTE"
                << "KEYCODE_0"
                << "KEYCODE_1"
                << "KEYCODE_2"
                << "KEYCODE_3"
                << "KEYCODE_4"
                << "KEYCODE_5"
                << "KEYCODE_6"
                << "KEYCODE_7"
                << "KEYCODE_8"
                << "KEYCODE_9"
                << "KEYCODE_10"
                << "KEYCODE_11"
                << "KEYCODE_12"
                << "KEYCODE_A"
                << "KEYCODE_B"
                << "KEYCODE_C"
                << "KEYCODE_D"
                << "KEYCODE_E"
                << "KEYCODE_F"
                << "KEYCODE_G"
                << "KEYCODE_H"
                << "KEYCODE_I"
                << "KEYCODE_J"
                << "KEYCODE_K"
                << "KEYCODE_L"
                << "KEYCODE_M"
                << "KEYCODE_N"
                << "KEYCODE_O"
                << "KEYCODE_P"
                << "KEYCODE_Q"
                << "KEYCODE_R"
                << "KEYCODE_S"
                << "KEYCODE_T"
                << "KEYCODE_U"
                << "KEYCODE_V"
                << "KEYCODE_W"
                << "KEYCODE_X"
                << "KEYCODE_Y"
                << "KEYCODE_Z"
                << "KEYCODE_APP_SWITCH"
                << "FLAG_SOFT_KEYBOARD"
                << "KEYCODE_ASSIST"
                << "KEYCODE_CALCULATOR"
                << "KEYCODE_DPAD_RIGHT"
                << "KEYCODE_WAKEUP";

    connect(this,   &ADB_widget::r_programm,   this,   &ADB_widget::run_program);
    //---

    //---
    timer_autoshot = new QTimer(this);
    connect(timer_autoshot, &QTimer::timeout,       this,   &ADB_widget::f_create_screenshot);
    //---

    connect(ui->btn_devices,            &QPushButton::clicked,  this,   &ADB_widget::f_devices);
    connect(ui->btn_create_screenshot,  &QPushButton::clicked,  this,   &ADB_widget::f_create_screenshot);
    connect(ui->btn_tap,                &QPushButton::clicked,  this,   &ADB_widget::f_screen_tap);

    connect(ui->btn_swipe_LR,           &QPushButton::clicked,  this,   &ADB_widget::f_test_swipe_LR);
    connect(ui->btn_swipe_RL,           &QPushButton::clicked,  this,   &ADB_widget::f_test_swipe_RL);
    connect(ui->btn_swipe_UD,           &QPushButton::clicked,  this,   &ADB_widget::f_test_swipe_UD);
    connect(ui->btn_swipe_DU,           &QPushButton::clicked,  this,   &ADB_widget::f_test_swipe_DU);

    connect(ui->btn_adb,                &QPushButton::clicked,  this,   &ADB_widget::f_adb);
    connect(ui->btn_test,               &QPushButton::clicked,  this,   &ADB_widget::f_test);

    connect(ui->le_cmd,                 &QLineEdit::returnPressed,  this,   &ADB_widget::run_cmd);
    connect(ui->btn_cmd,                &QPushButton::clicked,      this,   &ADB_widget::run_cmd);

    connect(ui->cb_auto,    &QCheckBox::toggled,    this,   &ADB_widget::f_auto_shot);

    connect(ui->btn_1,  &QToolButton::clicked,  this,   &ADB_widget::f_1);
    connect(ui->btn_2,  &QToolButton::clicked,  this,   &ADB_widget::f_2);
    connect(ui->btn_3,  &QToolButton::clicked,  this,   &ADB_widget::f_3);
    connect(ui->btn_4,  &QToolButton::clicked,  this,   &ADB_widget::f_4);

    ui->cb_commands->addItems(sl_commands);
    ui->btn_adb->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));

    //ui->cb_auto->setProperty(NO_BLOCK, true);
}
//--------------------------------------------------------------------------------
void ADB_widget::f_get_data(const QByteArray &data)
{
    emit trace("get_data");
    emit trace(data.data());
}
//--------------------------------------------------------------------------------
void ADB_widget::create_process(void)
{
    myProcess = new QProcess(this);
    connect(myProcess,  SIGNAL(started()),                      this,   SLOT(started()));
    connect(myProcess,  SIGNAL(finished(int)),                  this,   SLOT(finished(int)));
    connect(myProcess,  SIGNAL(error(QProcess::ProcessError)),  this,   SLOT(process_error(QProcess::ProcessError)));
    connect(myProcess,  SIGNAL(readyReadStandardOutput()),      this,   SLOT(readData()));
    connect(myProcess,  SIGNAL(readyReadStandardError()),       this,   SLOT(readData()));
}
//--------------------------------------------------------------------------------
void ADB_widget::run_program(const QString &program,
                             const QStringList &arguments)
{
    if(program.isEmpty())
    {
        emit error("program is empty");
        return;
    }

    f_busy = true;
    myProcess->start(program, arguments);
}
//--------------------------------------------------------------------------------
void ADB_widget::readData(void)
{
    emit info(QString("==> bytesAvailable: %1").arg(myProcess->bytesAvailable()));
    if(binary_data == false)
    {
        QString output = myProcess->readAllStandardOutput();
        QStringList lines = output.split("\n");
        emit debug(QString("received %1 bytes").arg(output.size()));
        emit debug(QString("lines.size() %1").arg(lines.size()));
        sl_data.clear();
        for(int n=0; n<lines.size(); n++)
        {
            QString line = lines.at(n);
            if(line.contains("List of devices attached") == false)
            {
                // emit debug(QString("line [%1]").arg(line));
                QString temp = line.remove("\t").remove("device").trimmed();
                // emit debug(QString("temp [%1]").arg(temp));
                if(temp.isEmpty() == false)
                {
                    sl_data.append(temp);
                }
            }
            emit info(QString("%1").arg(line));
        }
    }
}
//--------------------------------------------------------------------------------
void ADB_widget::started(void)
{
    block_interface(true);
    emit info("Процесс начат!");
}
//--------------------------------------------------------------------------------
void ADB_widget::finished(int result)
{
    block_interface(false);

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
void ADB_widget::process_error(QProcess::ProcessError p_error)
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
bool ADB_widget::f_devices(void)
{
    QString program = PROG_PROCESS;
    QStringList arguments;

    arguments << "devices";
    run_program(program, arguments);
    while(f_busy)
    {
        QCoreApplication::processEvents();
    }
    if(process_result == 0)
    {
        ui->cb_devices->clear();
        if(sl_data.isEmpty() == false)
        {
            ui->cb_devices->addItems(sl_data);  //TODO надо умнее
        }
    }

    return (process_result == 0);
}
//--------------------------------------------------------------------------------
void ADB_widget::f_create_screenshot(void)
{
    if(ui->cb_devices->currentText().isEmpty())
    {
        emit error("device not selected");
        return;
    }

    binary_data = true;

    QElapsedTimer timer;
    timer.start();
    f_get_screeshot();

    QByteArray ba = myProcess->readAllStandardOutput();

    QPixmap pixmap;
    pixmap.loadFromData(ba, "PNG");

    emit info(QString("Elapsed %1 msec").arg(timer.elapsed()));
    f_show_screeshot(pixmap);

    emit info("OK");
    binary_data = false;
}
//--------------------------------------------------------------------------------
bool ADB_widget::f_swipe(int x1, int y1, int x2, int y2, int delay)
{
    if(ui->cb_devices->currentText().isEmpty())
    {
        emit error("device not selected");
        return false;
    }
    //PROG_PROCESS shell input swipe x1 y1 x2 y2 sss

    QString program = PROG_PROCESS;
    QStringList arguments;

    arguments << "-s" << ui->cb_devices->currentText();
    arguments << "shell";
    arguments << "input";
    arguments << "swipe";
    arguments << QString("%1").arg(x1);
    arguments << QString("%1").arg(y1);
    arguments << QString("%1").arg(x2);
    arguments << QString("%1").arg(y2);
    arguments << QString("%1").arg(delay);

    run_program(program, arguments);
    while(f_busy)
    {
        QCoreApplication::processEvents();
    }
    if(process_result == 0)
    {
        f_create_screenshot();
    }
    return (process_result == 0);
}
//--------------------------------------------------------------------------------
bool ADB_widget::f_test_swipe_LR(void)
{
    return f_swipe(10,
                   s_height / 2,
                   s_width / 2,
                   s_height / 2,
                   100);
}
//--------------------------------------------------------------------------------
bool ADB_widget::f_test_swipe_RL(void)
{
    return f_swipe(s_width / 2,
                   s_height / 2,
                   10,
                   s_height / 2,
                   100);
}
//--------------------------------------------------------------------------------
bool ADB_widget::f_test_swipe_UD(void)
{
    return f_swipe(s_width / 2,
                   10,
                   s_width / 2,
                   s_height / 2,
                   100);
}
//--------------------------------------------------------------------------------
bool ADB_widget::f_test_swipe_DU(void)
{
    return f_swipe(s_width / 2,
                   s_height / 2,
                   s_width / 2,
                   10,
                   100);
}
//--------------------------------------------------------------------------------
bool ADB_widget::f_adb(void)
{
    if(ui->cb_devices->currentText().isEmpty())
    {
        emit error("device not selected");
        return false;
    }

    QString program = PROG_PROCESS;
    QStringList arguments;

    // https://developer.android.com/reference/android/view/KeyEvent

    arguments << "-s" << ui->cb_devices->currentText();
    arguments << "shell";
    arguments << "input";
    arguments << "keyevent";
    arguments << ui->cb_commands->currentText();

    QElapsedTimer timer;
    timer.start();

    run_program(program, arguments);
    while(f_busy)
    {
        QCoreApplication::processEvents();
    }

    emit info(QString("Elapsed %1 msec").arg(timer.elapsed()));

    if(process_result == 0)
    {
        f_create_screenshot();
    }
    return (process_result == 0);
}
//--------------------------------------------------------------------------------
bool ADB_widget::f_test(void)
{
    QStringList arguments;
    QString device = ui->cb_devices->currentText();

#if 0
    arguments.clear();
    arguments << "-s" << device;
    arguments << "push";
    arguments << "/dev/shm/scrcpy-server";
    arguments << "/data/local/tmp/scrcpy-server.jar";

    test_run(arguments);
#endif

#if 1
    arguments.clear();
    arguments << "-s" << device;
    arguments << "reverse";
    arguments << "localabstract:scrcpy";
    arguments << "tcp:27183";

    test_run(arguments);
#endif

#if 1
    arguments.clear();
    arguments << "-s" << device;
    arguments << "shell";
    arguments << "CLASSPATH=/data/local/tmp/scrcpy-server.jar";
    arguments << "app_process";
    arguments << "/";
    arguments << "com.genymobile.scrcpy.Server";
    arguments << "1.14";
    arguments << "info";
    arguments << "1080";
    arguments << "8000000";
    arguments << "60";
    arguments << "-1";
    arguments << "false";
    arguments << "-";

    test_run(arguments);
#endif

#if 1
    arguments.clear();
    arguments << "-s" << device;
    arguments << "reverse";
    arguments << "--remove";
    arguments << "localabstract:scrcpy";

    test_run(arguments);
#endif

    return true;
}
//--------------------------------------------------------------------------------
void ADB_widget::f_start(void)
{
    server->createServerOnPort(QHostAddress::LocalHost, static_cast<quint16>(ui->sb_port->value()));
}
//--------------------------------------------------------------------------------
void ADB_widget::f_stop(void)
{
    server->closeServer();
}
//--------------------------------------------------------------------------------
void ADB_widget::f_1(void)
{
    QStringList arguments;

    arguments.clear();
    arguments << "-s" << ui->cb_devices->currentText();
    arguments << "push";
    arguments << "scrcpy-server";
    arguments << "/data/local/tmp/scrcpy-server.jar";

    test_run(arguments);
}
//--------------------------------------------------------------------------------
void ADB_widget::f_2(void)
{
    QStringList arguments;

    arguments.clear();
    arguments << "-s" << ui->cb_devices->currentText();
    arguments << "reverse";
    arguments << "localabstract:scrcpy";
    arguments << "tcp:27183";

    test_run(arguments);
}
//--------------------------------------------------------------------------------
void ADB_widget::f_3(void)
{
    QStringList arguments;

    arguments.clear();
    arguments << "-s" << ui->cb_devices->currentText();
    arguments << "shell";
    arguments << "CLASSPATH=/data/local/tmp/scrcpy-server.jar";
    arguments << "app_process";
    arguments << "/";
    arguments << "com.genymobile.scrcpy.Server";
    arguments << "1.14";
    arguments << "info";
    arguments << "1080";
    arguments << "8000000";
    arguments << "60";
    arguments << "-1";
    arguments << "false";
    arguments << "-";

    test_run(arguments);
}
//--------------------------------------------------------------------------------
void ADB_widget::f_4(void)
{
    QStringList arguments;

    arguments.clear();
    arguments << "-s" << ui->cb_devices->currentText();
    arguments << "reverse";
    arguments << "--remove";
    arguments << "localabstract:scrcpy";

    test_run(arguments);
}
//--------------------------------------------------------------------------------
bool ADB_widget::test_run(const QStringList &arguments)
{
    if(ui->cb_devices->currentText().isEmpty())
    {
        emit error("device not selected");
        return false;
    }

    QString program = PROG_PROCESS;

    QElapsedTimer timer;
    timer.start();

    run_program(program, arguments);
    while(f_busy)
    {
        QCoreApplication::processEvents();
    }

    emit info(QString("process_result: %1").arg(process_result));
    return true;
}
//--------------------------------------------------------------------------------
bool ADB_widget::f_get_file_screeshot(void)
{
    if(ui->cb_devices->currentText().isEmpty())
    {
        emit error("device not selected");
        return false;
    }

    QString program = PROG_PROCESS;
    QStringList arguments;

    arguments << "-s" << ui->cb_devices->currentText();
    arguments << "pull";
    arguments << "/sdcard/screencap.png";

    run_program(program, arguments);
    while(f_busy)
    {
        QCoreApplication::processEvents();
    }
    return (process_result == 0);
}
//--------------------------------------------------------------------------------
void ADB_widget::f_screen_tap(void)
{
    f_tap(ui->sb_pos_x->value(),
          ui->sb_pos_y->value());
}
//--------------------------------------------------------------------------------
bool ADB_widget::f_tap(int pos_x, int pos_y)
{
    if(ui->cb_devices->currentText().isEmpty())
    {
        emit error("device not selected");
        return false;
    }
    //PROG_PROCESS shell input tap 100 100

    QString program = PROG_PROCESS;
    QStringList arguments;

    arguments << "-s" << ui->cb_devices->currentText();
    arguments << "shell";
    arguments << "input";
    arguments << "tap";
    arguments << QString("%1").arg(pos_x);
    arguments << QString("%1").arg(pos_y);

    run_program(program, arguments);
    while(f_busy)
    {
        QCoreApplication::processEvents();
    }
    if(process_result == 0)
    {
        f_create_screenshot();
    }
    return (process_result == 0);
}
//--------------------------------------------------------------------------------
void ADB_widget::run_cmd(void)
{
    if(ui->cb_devices->currentText().isEmpty())
    {
        emit error("device not selected");
        return;
    }

    QString program = PROG_PROCESS;
    QStringList arguments;

    // https://gist.github.com/davidnunez/1404789
    // adb shell 'pm list packages' | sed 's/.*://g'

    arguments << "-s" << ui->cb_devices->currentText();
    arguments << "shell";
    arguments << ui->le_cmd->text();

    block_interface(true);
    run_program(program, arguments);
    while(f_busy)
    {
        QCoreApplication::processEvents();
    }
    block_interface(false);
    return;
}
//--------------------------------------------------------------------------------
void ADB_widget::f_auto_shot(bool state)
{
    if(ui->cb_devices->currentText().isEmpty())
    {
        emit error("device not selected");
        ui->cb_auto->setChecked(false);
        return;
    }

    if(state)
    {
        ui->btn_create_screenshot->setDisabled(true);
        timer_autoshot->start(ui->sb_interval->value());
    }
    else
    {
        ui->btn_create_screenshot->setEnabled(true);
        timer_autoshot->stop();
    }
}
//--------------------------------------------------------------------------------
void ADB_widget::f_show_screeshot(QPixmap pixmap)
{
    s_width = pixmap.width();
    s_height = pixmap.height();

    ui->sb_pos_x->setRange(0, s_width);
    ui->sb_pos_y->setRange(0, s_height);

    emit info(QString("size %1 %2").arg(s_width).arg(s_height));
    emit set_pixmap(pixmap);
}
//--------------------------------------------------------------------------------
bool ADB_widget::f_get_screeshot(void)
{
    if(ui->cb_devices->currentText().isEmpty())
    {
        emit error("device not selected");
        return false;
    }

    //PROG_PROCESS exec-out screencap -p
    QString program = PROG_PROCESS;
    QStringList arguments;

    arguments << "-s" << ui->cb_devices->currentText();
    arguments << "exec-out";
#if 1
    arguments << "screencap";
#else
    arguments << "screenrecord";
    arguments << "–bit-rate";
    arguments << "12000000";
#endif
    arguments << "-p";

    run_program(program, arguments);
    while(f_busy)
    {
        QCoreApplication::processEvents();
    }
    return (process_result == 0);
}
//--------------------------------------------------------------------------------
bool ADB_widget::copy_file(const QString &src_filename,
                        const QString &dst_filename)
{
    QFile src_file(src_filename);
    QFile dst_file(dst_filename);

    if(dst_file.exists())
    {
        return true;
    }

    if(!src_file.open(QIODevice::ReadOnly))
    {
        emit error(QString("file %1 not open").arg(src_filename));
        return false;
    }
    if(!src_file.copy(dst_filename))
    {
        emit error(QString("file %1 not created").arg(dst_filename));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
void ADB_widget::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool ADB_widget::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void ADB_widget::load_setting(void)
{
    ui->btn_devices->click();

    int interval = 0;
    int pos_x = 0;
    int pos_y = 0;
    bool ok = false;

    ok = load_int(P_INTERVAL, &interval);
    if(ok)
    {
        ui->sb_interval->setValue(interval);
    }
    ok = load_int(P_POS_Y, &pos_y);
    if(ok)
    {
        ui->sb_pos_x->setValue(pos_x);
    }
    ok = load_int(P_POS_X, &pos_x);
    if(ok)
    {
        ui->sb_pos_y->setValue(pos_y);
    }
}
//--------------------------------------------------------------------------------
void ADB_widget::save_setting(void)
{
    save_int(P_INTERVAL,    ui->sb_interval->value());
    save_int(P_POS_X,       ui->sb_pos_x->value());
    save_int(P_POS_Y,       ui->sb_pos_y->value());
}
//--------------------------------------------------------------------------------
