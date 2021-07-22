/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#include "mylogger.hpp"
//--------------------------------------------------------------------------------
extern MyLogger *logger;
//--------------------------------------------------------------------------------
void myMessageOutput(QtMsgType type,
                     const QMessageLogContext &context,
                     const QString &msg)
{
    switch (type)
    {
    case QtDebugMsg:    if(logger) logger->sender_debug(QString("QtDebugMsg: %1").arg(msg));       break;
    case QtInfoMsg:     if(logger) logger->sender_info(QString("QtInfoMsg: %1").arg(msg));         break;
    case QtWarningMsg:  if(logger) logger->sender_warning(QString("QtWarningMsg: %1").arg(msg));   break;
    case QtCriticalMsg: if(logger) logger->sender_critical(QString("QtCriticalMsg: %1").arg(msg)); break;
    case QtFatalMsg:    if(logger) logger->sender_fatal(QString("QtFatalMsg: %1").arg(msg));       break;
    }
    Q_UNUSED(context);
}
//--------------------------------------------------------------------------------
MyLogger::MyLogger(QObject *parent) : QObject(parent)
{

}
//--------------------------------------------------------------------------------
MyLogger::~MyLogger()
{

}
//--------------------------------------------------------------------------------
void MyLogger::sender_info(const QString  &text)
{
    emit info(text);
}
//--------------------------------------------------------------------------------
void MyLogger::sender_debug(const QString &text)
{
    emit debug(text);
}
//--------------------------------------------------------------------------------
void MyLogger::sender_warning(const QString &text)
{
    emit error(text);
}
//--------------------------------------------------------------------------------
void MyLogger::sender_critical(const QString &text)
{
    emit error(text);
}
//--------------------------------------------------------------------------------
void MyLogger::sender_fatal(const QString &text)
{
    emit error(text);
}
//--------------------------------------------------------------------------------
