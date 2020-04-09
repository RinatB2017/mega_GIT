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
#define MIN_WIDTH   10
#define MAX_WIDTH   256
#define MIN_HEIGHT  10
#define MAX_HEIGHT  256

#define S_SIZE_W        "size_w"
#define S_SIZE_H        "size_h"
#define S_SIZE_BORDER   "size_border"
#define S_BORDER_COLOR  "border_color"
#define S_BACKGROUND_COLOR  "background_color"
#define S_MARGIN_T  "margin_t"
#define S_MARGIN_B  "margin_b"
#define S_MARGIN_L  "margin_l"
#define S_MARGIN_R  "margin_r"
#define S_TRANSPARENT_BORDER    "transparent_border"
#define S_TRANSPARENT_BACKGROUND    "transparent_background"
#define S_RB_ECLIPSE        "rb_eclipse"
#define S_RB_RECTANGLE      "rb_rectangle"
#define S_RB_ARROW_LEFT     "rb_arrow_left"
#define S_RB_ARROW_RIGHT    "rb_arrow_right"
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
    QColor border_color = Qt::black;
    QColor background_color = Qt::black;

    void init(void);

    void set_width(int width);
    int get_width(void);

    void set_height(int height);
    int get_height(void);

    void set_border(int size);
    int get_border(void);

    void set_border_color(void);
    void set_background_color(void);
    void set_color(QToolButton *btn, QColor color);

    void save_picture_to(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // CREATORBUTTON_HPP
