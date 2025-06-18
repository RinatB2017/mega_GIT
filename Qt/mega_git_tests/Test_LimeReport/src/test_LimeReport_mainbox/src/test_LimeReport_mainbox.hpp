/*********************************************************************************
**                                                                              **
**     Copyright (C) 2025                                                       **
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
#include "ui_test_LimeReport_mainbox.h"
//--------------------------------------------------------------------------------
#include <QPointer>
#include <QtPrintSupport/QPrinter>
#include <QStringListModel>

#include "LimeReport"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "myfiledialog.hpp"
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

private slots:
    void choice_test(void);
    void choice_programm(void);
    bool test(void);

private:
    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;

    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (MainBox::*func)(void);
    } CMD_t;
    QList<CMD> test_commands;
    QList<CMD> programm_commands;

    QPointer<QToolBar> test_bar;
    QPointer<QToolBar> programm_bar;
    QPointer<QComboBox> cb_test;
    QPointer<QComboBox> cb_programm;

    // LimeReport::ReportEngine *report = nullptr;
    LimeReport::ReportEngine *report;

    void init(void);

    void create_test_bar(void);
    void create_programm_bar(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
