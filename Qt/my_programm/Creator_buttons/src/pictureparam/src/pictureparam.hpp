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
#ifndef PICTUREPARAM_HPP
#define PICTUREPARAM_HPP
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
    class PictureParam;
}
//--------------------------------------------------------------------------------
class PictureParam : public MyWidget
{
    Q_OBJECT

signals:
    void picture_update(void);

public:
    explicit PictureParam(QWidget *parent = nullptr);
    ~PictureParam();

    void set_caption(const QString &caption);

    int get_margen_t(void);
    int get_margen_b(void);
    int get_margen_l(void);
    int get_margen_r(void);

    int get_border(void);

    bool get_active(void);

    bool get_border_color_transparent(void);

    void set_border_color(QColor color);

    QColor get_border_color(void);

private:
    Ui::PictureParam *ui;

    QColor border_color = Qt::black;

    void init(void);

    void set_color(QToolButton *btn, QColor color);

    void set_border_color(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // PICTUREPARAM_HPP
