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
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include "ui_ups_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    virtual ~MainBox();

public slots:
    bool test(void);

private slots:
    void choice_test(void);

    void read_data(void);
    void read_error(void);

    void started(void);
    void finished(int result, QProcess::ExitStatus exitStatus);

    void process_error(QProcess::ProcessError p_error);

    void run(void);
    void run_upsdrvctl(void);

private:
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (MainBox::*func)(void);
    } CMD_t;
    typedef struct DATA
    {
        QString data_name;
        QLCDNumber *lcd_display;
        int curve_id;
    } DATA_t;

    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;

    QPointer<QToolBar> testbar;
    QPointer<QComboBox> cb_test;
    QList<CMD> commands;

    QList<DATA> display_data;

    QProcess *process = nullptr;
    QPointer<QTimer> timer;

    void init(void);
    void init_display_data(void);
    void init_grapher(void);
    void init_timer(void);
    void createTestBar(void);

    void prepare_QProcess(void);
    void show_data(const QString &line);

    void t_start(void);
    void t_stop(void);
    void t_update(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
