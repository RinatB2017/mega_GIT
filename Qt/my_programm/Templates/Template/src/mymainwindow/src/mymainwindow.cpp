/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#include <QDebug>
//--------------------------------------------------------------------------------
#include "mymainwindow.hpp"
//--------------------------------------------------------------------------------
QFile *MyMainWindow::m_logFile;
//--------------------------------------------------------------------------------
MyMainWindow::MyMainWindow(MainWindow *parent) :
    MainWindow(parent)
{

}
//--------------------------------------------------------------------------------
void MyMainWindow::install_handler(void)
{
    m_logFile = new QFile(QString("%1.log").arg(qAppName()));
    m_logFile->open(QFile::Append | QFile::Text);

#ifdef HAVE_QT5
    qInstallMessageHandler(messageHandler);
#else
    qInstallMsgHandler(messageHandler);
#endif
}
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
void MyMainWindow::messageHandler(QtMsgType type,
                                  const QMessageLogContext &context,
                                  const QString &msg)
{
    // Открываем поток записи в файл
    QTextStream out(m_logFile);
    // Записываем дату записи
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    // По типу определяем, к какому уровню относится сообщение
    switch (type)
    {
    case QtInfoMsg:     out << "INF ";  break;
    case QtDebugMsg:    out << "DBG ";  break;
    case QtWarningMsg:  out << "WRN ";  break;
    case QtCriticalMsg: out << "CRT ";  break;
    case QtFatalMsg:    out << "FTL ";  break;
    }
    // Записываем в вывод категорию сообщения и само сообщение
    out << context.category
        << ": "
        << msg
        << endl;
    out.flush();    // Очищаем буферизированные данные
}
#else
void MyMainWindow::messageHandler(QtMsgType type,
                                  const char *msg)
{
    // Открываем поток записи в файл
    QTextStream out(m_logFile);
    // Записываем дату записи
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    // По типу определяем, к какому уровню относится сообщение
    switch (type)
    {
    case QtDebugMsg:    out << "DBG ";  break;
    case QtWarningMsg:  out << "WRN ";  break;
    case QtCriticalMsg: out << "CRT ";  break;
    case QtFatalMsg:    out << "FTL ";  break;
    }
    // Записываем в вывод категорию сообщения и само сообщение
    out << ": "
        << msg
        << endl;
    out.flush();    // Очищаем буферизированные данные
}
#endif
//--------------------------------------------------------------------------------
MyMainWindow::~MyMainWindow()
{
#ifdef QT_DEBUG
    qDebug() << "~MyMainWindow()";
#endif
}
//--------------------------------------------------------------------------------
