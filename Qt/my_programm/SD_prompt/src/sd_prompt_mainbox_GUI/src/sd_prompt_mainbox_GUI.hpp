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
#ifndef MAINBOX_GUI_HPP
#define MAINBOX_GUI_HPP
//--------------------------------------------------------------------------------
#include "ui_sd_prompt_mainbox_GUI.h"
//--------------------------------------------------------------------------------
#include <QPointer>
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox_GUI;
}
//--------------------------------------------------------------------------------
class MainBox_GUI : public MyWidget
{
    Q_OBJECT

signals:
    void s_create_positive_prompt(void);
    void s_create_negative_prompt(void);

public:
    explicit MainBox_GUI(QWidget *parent,
                         MySplashScreen *splash);
    virtual ~MainBox_GUI();

    QString get_positive_prompt(void);
    QString get_negative_prompt(void);

private:
    QPointer<MySplashScreen> splash;
    Ui::MainBox_GUI *ui;

    void init(void);
    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_GUI_HPP
