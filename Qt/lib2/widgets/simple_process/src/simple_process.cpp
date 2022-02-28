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
#include "simple_process.hpp"
//--------------------------------------------------------------------------------
Simple_process::Simple_process(QWidget *parent) :
    MyWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
Simple_process::~Simple_process()
{
    if(myProcess)
    {
        delete myProcess;
    }
}
//--------------------------------------------------------------------------------
void Simple_process::init(void)
{
    myProcess = new QProcess(this);
    connect(myProcess,  &QProcess::started,                 this,   &Simple_process::started);
    connect(myProcess,  &QProcess::readyReadStandardOutput, this,   &Simple_process::read_data);
    connect(myProcess,  &QProcess::readyReadStandardError,  this,   &Simple_process::read_error);
    connect(myProcess,  static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this,       static_cast<void (Simple_process::*)(int, QProcess::ExitStatus)>(&Simple_process::finished));
    connect(myProcess,  static_cast<void (QProcess::*)(QProcess::ProcessError)>(&QProcess::errorOccurred),
            this,       static_cast<void (Simple_process::*)(QProcess::ProcessError)>(&Simple_process::process_error));
}
//--------------------------------------------------------------------------------
void Simple_process::programm_start(const QString &program,
                                    const QStringList &arguments)
{
    emit trace(Q_FUNC_INFO);
    foreach (QString arg, arguments)
    {
        emit debug(arg);
    }
    myProcess->start(program, arguments);
}
//--------------------------------------------------------------------------------
void Simple_process::setProcessEnvironment(QProcessEnvironment env)
{
    myProcess->setProcessEnvironment(env);
}
//--------------------------------------------------------------------------------
void Simple_process::started(void)
{
    emit info("Процесс начат!");
    timer.start();
}
//--------------------------------------------------------------------------------
void Simple_process::finished(int result, QProcess::ExitStatus exitStatus)
{
    emit info("Процесс завершен!");
    emit debug(QString("code %1").arg(result));
    switch (exitStatus)
    {
    case QProcess::NormalExit:
        emit info("The process exited normally.");
        emit info(QString("Elapsed: %1 sec")
                  .arg(timer.elapsed() / 1000.0));
        break;
    case QProcess::CrashExit:
        emit error("The process crashed.");
        break;
    }

    emit s_finished(result, exitStatus);
}
//--------------------------------------------------------------------------------
void Simple_process::process_error(QProcess::ProcessError err)
{
    switch(err)
    {
    case QProcess::FailedToStart:   emit error("process_error: FailedToStart");    break;
    case QProcess::Crashed:         emit error("process_error: Crashed");          break;
    case QProcess::Timedout:        emit error("process_error: Timedout");         break;
    case QProcess::ReadError:       emit error("process_error: ReadError");        break;
    case QProcess::WriteError:      emit error("process_error: WriteError");       break;
    case QProcess::UnknownError:    emit error("process_error: UnknownError");     break;
    }
}
//--------------------------------------------------------------------------------
void Simple_process::read_data(void)
{
    QString output = myProcess->readAllStandardOutput();
    emit info(output);
}
//--------------------------------------------------------------------------------
void Simple_process::read_error(void)
{
    QString output = myProcess->readAllStandardError();
    emit info(output);
}
//--------------------------------------------------------------------------------
void Simple_process::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool Simple_process::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Simple_process::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void Simple_process::save_setting(void)
{

}
//--------------------------------------------------------------------------------
