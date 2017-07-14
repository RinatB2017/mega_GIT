/*
 * Copyright 2010 Alexey Michurin
 *
 * This file is part of FairSeaBattle.
 *
 * FairSeaBattle is free software: you can redistribute
 * it and/or modify it under the terms of the
 * GNU General Public License as published by
 * the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * FairSeaBattle is distributed in the hope that
 * it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the
 * GNU General Public License along with FairSeaBattle.
 * If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef BS_GUI_ARENAWIDGET_H
#define BS_GUI_ARENAWIDGET_H

#include <QFrame>
#include <QTimerEvent>
#include <QLinkedList>

#include "map.h"
#include "flags_colors.h"


class ArenaCellWidget : public QFrame {

    Q_OBJECT

private:
    int tik_count;
    QColor target_color;
    QColor source_color;
    ColorStrategyMode color_update_mode;
    int timer_id;

    void update_color();
    void set_color(QColor const &);
    void set_mix_color(QColor const &, QColor const &);
    void one_step();

protected:
    void mousePressEvent(QMouseEvent * event);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void timerEvent(QTimerEvent * event);

public:
    ArenaCellWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);

//    ArenaCellWidget(const ArenaCellWidget &);
//    ArenaCellWidget & operator=(const ArenaCellWidget &);

    void changeColor(ColorStrategy const & color);

signals:
    void click(); // signal say "i'am clicked"
};

// ========================================================

class ArenaCellMediator : public QObject {

    Q_OBJECT

private:
    BSPoint location;

public:
    ArenaCellMediator(); // not inmplemented
    ArenaCellMediator(QObject * qo, const BSPoint & p);

public slots:
    void click();

signals:
    void click(BSPoint); // signal say "someone click"
};

// ========================================================

class ArenaWidget : public QFrame {

    Q_OBJECT

private:
    BSMap<ArenaCellWidget *> fmap;
    bool is_active;
    int timer_id;

    void one_step();

protected:
    void timerEvent(QTimerEvent * event);

public:
    ArenaWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);

public slots:
    void accept_click(BSPoint);

    void change_color(BSPoint p, ColorStrategy c);
    void clear();
    void activate();
    void deactivate();

signals:
    void click(BSPoint); // signal say "someone click"
};

#endif
