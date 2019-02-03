/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#ifndef IMITATOR_PET_HPP
#define IMITATOR_PET_HPP
//--------------------------------------------------------------------------------
#include <stdint.h>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
typedef struct point_data
{
    int   brg;
    qreal x1;
    qreal y1;
    qreal x2;
    qreal y2;
} point_data_t;
//--------------------------------------------------------------------------------
namespace Ui {
    class Imitator_PET;
}
//--------------------------------------------------------------------------------
typedef struct brg_data
{
    int brg_1;
    int brg_2;
} brg_data_t;
//--------------------------------------------------------------------------------
class MySplashScreen;
class QToolButton;
class QToolBar;
class QComboBox;
class QPixmap;
//--------------------------------------------------------------------------------
class Imitator_PET : public MyWidget
{
    Q_OBJECT

public:
    explicit Imitator_PET(QWidget *parent);
    ~Imitator_PET();

    QString convert_int_to_string(uint32_t value);

signals:
    void block_widget(bool);

private slots:
    void choice_filename(void);
    void create_filename(void);
    void set_source(void);

    void generate_control_events(void);
    void show_control_events(void);

private:
    Ui::Imitator_PET *ui;

    QComboBox *cb_test;
    QList<struct point_data> points;

    QList<struct pet_event> control_events;

    qreal main_source_x;
    qreal main_source_y;

    qreal control_source_x;
    qreal control_source_y;

    QList<struct brg_data> brg_data_lists;

    void init(void);
    void create_widgets(void);

    void calc_line(qreal center_x,
                   qreal center_y,
                   qreal angle,
                   qreal radius,
                   qreal *end_x,
                   qreal *end_y);
    bool check_brg(int index_brg,
                   qreal center_x,
                   qreal center_y,
                   qreal angle);

    void update_brg_doubles(void);

    qreal get_angle(qreal x1,
                    qreal y1,
                    qreal x2,
                    qreal y2);

    void redraw_all(void);
    void draw_main_picture(void);
    void draw_control_picture(void);

    static bool comp(const pet_event &e1, const pet_event &e2);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void changeEvent(QEvent *event);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
