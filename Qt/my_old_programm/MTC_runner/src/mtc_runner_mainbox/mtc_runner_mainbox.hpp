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
#include <QProcess>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class QToolButton;
class QToolBar;
class QComboBox;

class QCheckBox;
class QLineEdit;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent,
            MySplashScreen *splash);
    ~MainBox();

private slots:
    void run(void);

    void procfunc(void);
    void read_data(void);
    void read_error(void);

    void started(void);
    void finished(int result);

    void process_error(QProcess::ProcessError p_error);

    void choice_script(void);
    void auto_run(bool state);

private:
    MySplashScreen *splash = 0;
    Ui::MainBox *ui = 0;

    void init(void);

    QCheckBox *cb_auto_run = 0;
    QToolButton *btn_script = 0;
    QLineEdit *le_script_filename = 0;
    QToolButton *btn_run = 0;
    QProcess *proccess = 0;

    void createRunBar(void);
    void createScriptBar(void);

    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
