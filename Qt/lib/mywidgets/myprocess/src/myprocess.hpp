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
#ifndef MYPROCESS_HPP
#define MYPROCESS_HPP
//--------------------------------------------------------------------------------
#include <QProcess>
//--------------------------------------------------------------------------------
class MyProcess : public QProcess
{
public:
    explicit MyProcess(QObject *parent = nullptr);
    ~MyProcess();

private:
    void init(void);

protected:
    virtual void started(void) = 0;
    virtual qint64 read_data(void) = 0;
    virtual qint64 read_error(void) = 0;
    virtual void finished(int exitCode, QProcess::ExitStatus exitStatus) = 0;
    virtual void process_error(QProcess::ProcessError error) = 0;
};
//--------------------------------------------------------------------------------
#endif // MYPROCESS_HPP
