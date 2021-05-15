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
#include "myprocess.hpp"
//--------------------------------------------------------------------------------
MyProcess::MyProcess(QObject *parent) :
    QProcess(parent)
{
    init();
}
//--------------------------------------------------------------------------------
MyProcess::~MyProcess()
{

}
//--------------------------------------------------------------------------------
void MyProcess::init(void)
{
    setProcessChannelMode(QProcess::SeparateChannels);
    //setReadChannel(QProcess::StandardOutput);

    connect(this,   &QProcess::started,                 this,   &MyProcess::started);
    connect(this,   &QProcess::readyReadStandardOutput, this,   &MyProcess::read_data);
    connect(this,   &QProcess::readyReadStandardError,  this,   &MyProcess::read_error);
    connect(this,   static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this,   static_cast<void (MyProcess::*)(int, QProcess::ExitStatus)>(&MyProcess::finished));
    connect(this,   static_cast<void (QProcess::*)(QProcess::ProcessError)>(&QProcess::errorOccurred),
            this,   static_cast<void (MyProcess::*)(QProcess::ProcessError)>(&MyProcess::process_error));

//    connect(this,   &QProcess::started,                 this,   &MyProcess::started);
//    connect(this,   &QProcess::readyReadStandardOutput, this,   &MyProcess::read_data);
//    connect(this,   &QProcess::readyReadStandardError,  this,   &MyProcess::read_error);
//    connect(this,   static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
//            this,   static_cast<void (MyProcess::*)(int, QProcess::ExitStatus)>(&MyProcess::finished));
//    connect(this,   static_cast<void (QProcess::*)(QProcess::ProcessError)>(&QProcess::errorOccurred),
//            this,   static_cast<void (MyProcess::*)(QProcess::ProcessError)>(&MyProcess::process_error));
}
//--------------------------------------------------------------------------------
