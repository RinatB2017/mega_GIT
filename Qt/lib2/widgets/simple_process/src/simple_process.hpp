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
#ifndef SIMPLE_PROCESS_HPP
#define SIMPLE_PROCESS_HPP
//--------------------------------------------------------------------------------
#include <QElapsedTimer>
#include <QPointer>
#include <QProcess>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class Simple_process : public MyWidget
{
    Q_OBJECT

signals:
    void s_finished(int result, QProcess::ExitStatus exitStatus);

public:
    explicit Simple_process(QWidget *parent = nullptr);
    ~Simple_process();

    void programm_start(const QString &program,
                        const QStringList &arguments);
    void setProcessEnvironment(QProcessEnvironment env);

private slots:
    void started();
    void finished(int result, QProcess::ExitStatus exitStatus);
    void process_error(QProcess::ProcessError err);
    void read_data();
    void read_error();

private:
    QPointer<QProcess> myProcess;
    QElapsedTimer timer;

    void init();

    void updateText();
    bool programm_is_exit();
    void load_setting();
    void save_setting();
};
//--------------------------------------------------------------------------------
#endif // SIMPLE_PROCESS_HPP
