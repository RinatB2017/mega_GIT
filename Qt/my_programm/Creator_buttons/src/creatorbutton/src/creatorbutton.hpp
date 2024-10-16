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
#ifndef CREATORBUTTON_HPP
#define CREATORBUTTON_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class CreatorButton;
}
//--------------------------------------------------------------------------------
class CreatorButton : public MyWidget
{
    Q_OBJECT

public:
    explicit CreatorButton(QWidget *parent = nullptr);
    ~CreatorButton();

private:
    Ui::CreatorButton *ui;

private slots:
    void create_picture(void);

private:
    void init(void);

    void set_width(int width);
    int get_width(void);

    void set_height(int height);
    int get_height(void);

    void save_picture_to(void);

    void set_color(QToolButton *btn, QColor color);
    void set_background_color(void);

    int border = 0;

    QColor background_color = Qt::black;

    int offset_t = 0;
    int offset_b = 0;
    int offset_l = 0;
    int offset_r = 0;

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // CREATORBUTTON_HPP
