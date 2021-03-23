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
#ifndef HSV_COLOR_DIALOG_HPP
#define HSV_COLOR_DIALOG_HPP
//--------------------------------------------------------------------------------
#include <QDialog>
//--------------------------------------------------------------------------------
namespace Ui {
    class HSV_color_dialog;
}
//--------------------------------------------------------------------------------
class HSV_color_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit HSV_color_dialog(QWidget *parent = nullptr);
    ~HSV_color_dialog();

    void set_color(QColor color);
    QColor get_color(void);

private:
    Ui::HSV_color_dialog *ui;
};
//--------------------------------------------------------------------------------
#endif // HSV_COLOR_DIALOG_HPP
