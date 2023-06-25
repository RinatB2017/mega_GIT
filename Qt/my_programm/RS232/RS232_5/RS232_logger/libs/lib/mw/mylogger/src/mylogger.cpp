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
    case QtDebugMsg:
#ifndef NO_LOG_QDEBUG
        if(logger) logger->sender_debug(msg);
#endif
        break;

    case QtInfoMsg:
#ifndef NO_LOG_QINFO
        if(logger) logger->sender_info(msg);
#endif
        break;

    case QtWarningMsg:
#ifndef NO_LOG_QTRACE
        if(logger) logger->sender_warning(msg);
#endif
        break;

    case QtCriticalMsg:
#ifndef NO_LOG_QTRACE
        if(logger) logger->sender_critical(msg);
        break;
#endif

    case QtFatalMsg:
#ifndef NO_LOG_QTRACE
        if(logger) logger->sender_fatal(msg);
#endif
        break;

    default:
        if(logger) logger->sender_info(msg);
        break;
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
