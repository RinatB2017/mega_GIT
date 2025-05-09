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
#include "ui_test_qml_mainbox_GUI.h"
//--------------------------------------------------------------------------------
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
    void btn_show_qml(void);

public:
    explicit MainBox_GUI(QWidget *parent,
                         MySplashScreen *splash);
    virtual ~MainBox_GUI();

    QString get_qml_text(void);
    void set_qml_text(const QString &text);

private:
    QPointer<MySplashScreen> splash;
    Ui::MainBox_GUI *ui;

    void init(void);
    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_GUI_HPP
