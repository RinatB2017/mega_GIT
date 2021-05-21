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
#ifndef HSV_WIDGET_HPP
#define HSV_WIDGET_HPP
//--------------------------------------------------------------------------------
#include <QElapsedTimer>
//--------------------------------------------------------------------------------
#include "myfiledialog.hpp"
#include "mywidget.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class HSV_widget;
}
//--------------------------------------------------------------------------------
class HSV_widget : public MyWidget
{
    Q_OBJECT

public:
    explicit HSV_widget(QWidget *parent = nullptr);
    virtual ~HSV_widget();

private:
    Ui::HSV_widget *ui;

    void init(void);

    void load_picture(void);
    void set_saturation(void);
    void set_value(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // HSV_WIDGET_HPP
