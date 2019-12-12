/*********************************************************************************
**                                                                              **
**     Copyright (C) 2019                                                       **
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
#ifndef OSCILLOSCOPEBOX_CONTROLS_HPP
#define OSCILLOSCOPEBOX_CONTROLS_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
namespace Ui {
    class Oscilloscopebox_controls;
}
//--------------------------------------------------------------------------------
class Oscilloscopebox_controls : public QWidget
{
    Q_OBJECT

signals:
    void s_color(int);
    void s_select(bool);

public:
    explicit Oscilloscopebox_controls(QWidget *parent = nullptr);
    ~Oscilloscopebox_controls();

    bool add_control(QColor color,  QString text);
    bool set_curve_color(int index, QColor color);
    bool set_curve_text(int index,  QString text);

    int get_active_index(void);

private slots:
    void click_color(void);
    void click_select(bool state);

    void check_buttons(bool state);

private:
    Ui::Oscilloscopebox_controls *ui;

    struct CURVE_BTNS {
        QToolButton *btn_color;
        QPushButton *btn_select;
    };
    QList<CURVE_BTNS> l_curves;
    int curves = 0;
    int current_index = -1;

    void init(void);
    void update_curves(void);
};
//--------------------------------------------------------------------------------
#endif // COLOR_CURVE_HPP
