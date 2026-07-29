/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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
#ifndef HISTOGRAMCURVEBOX_HPP
#define HISTOGRAMCURVEBOX_HPP
//--------------------------------------------------------------------------------
#include <QColorDialog>
#include <QLineEdit>
#include <QLabel>
#include <QEvent>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class Histogram_CurveBox : public MyWidget
{
    Q_OBJECT

public:
    explicit Histogram_CurveBox(QWidget *parent);

    void set_color(int r, int g, int b);
    void set_caption(const QString &caption);
    void set_index(int index);

    QString get_caption(bool *ok);
    void get_color(int *r, int *g, int *b);
    int get_index();

private slots:
    void change_color();

private:
    int index;
    QLineEdit *caption_curve = nullptr;
    QLabel *color_curve = nullptr;

    void updateText();
    bool programm_is_exit();
    void load_setting();
    void save_setting();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

};
//--------------------------------------------------------------------------------
#endif // HISTOGRAMCURVEBOX_HPP
