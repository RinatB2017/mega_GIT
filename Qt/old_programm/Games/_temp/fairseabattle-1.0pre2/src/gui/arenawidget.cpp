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
#include "arenawidget.h"
#include <QGridLayout>
#include <QSizePolicy>

template class BSMap<ArenaCellWidget>;

// ================================================

ArenaCellWidget::ArenaCellWidget(QWidget * parent, Qt::WindowFlags flags) :
    QFrame(parent, flags),
    tik_count(0),
    target_color(102, 102, 102),
    source_color(102, 102, 102),
    color_update_mode(update_mode_smooth),
    timer_id(0)
{
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(1);
    setAutoFillBackground(true);
    set_color(target_color);
}

// ---------------

void
ArenaCellWidget::mousePressEvent(QMouseEvent *) {
    emit click();
}

void
ArenaCellWidget::enterEvent(QEvent *) {
    update_color();
}

void
ArenaCellWidget::leaveEvent(QEvent *) {
    update_color();
}

void
ArenaCellWidget::timerEvent(QTimerEvent *) {
    one_step();
}

// ---------------

void
ArenaCellWidget::changeColor(ColorStrategy const & c) {
    source_color = palette().color(QPalette::Background);
    target_color = c.color();
    color_update_mode = c.mode();
    tik_count = 0;
    one_step();
}

// ---------------

void
ArenaCellWidget::one_step() {
    // устанавливаем заданное состояние
    switch (color_update_mode) {
        case update_mode_blink:
            if (tik_count % 2 == 0) {
                set_color(target_color);
            } else {
                set_mix_color(QColor(255, 255, 255), target_color);
            }
            break;
        case update_mode_smooth:
            set_mix_color(source_color, target_color);
            break;
    }
    // если закончили -- останавливаем таймер
    if (tik_count == 10 && timer_id != 0) {
        killTimer(timer_id);
        timer_id = 0;
    } else {
        // вырабатываем новое состояние и понимание, нежны ли ещё итерации
        ++tik_count;
        // запускаем таймер (если ещё не запущен)
        if (timer_id == 0) {
            timer_id = startTimer(100);
        }
    }
}

void
ArenaCellWidget::set_color(QColor const & c) {
    QPalette p(palette());
    p.setColor(QPalette::Background, c);
    setPalette(p);
    update_color();
}

void
ArenaCellWidget::set_mix_color(QColor const & s, QColor const & t) {
    int a(10 - tik_count);
    int b(tik_count);
    set_color(QColor(( a * s.red()   + b * t.red()   )/10,
                     ( a * s.green() + b * t.green() )/10,
                     ( a * s.blue()  + b * t.blue()  )/10 ));
}

void
ArenaCellWidget::update_color() {
    QPalette p(palette());
    QColor a = p.color(QPalette::Background);
    if (underMouse()) {
        p.setColor(QPalette::Light, a);
        p.setColor(QPalette::Dark, a);
    } else {
        p.setColor(QPalette::Light, a.lighter());
        p.setColor(QPalette::Dark, a.darker());
    }
    setPalette(p);
}

// ================================================

ArenaCellMediator::ArenaCellMediator(QObject * qo, const BSPoint & p) :
    QObject(qo),
    location(p)
{
}

void
ArenaCellMediator::click() {
    emit click(location);
}

// ================================================

ArenaWidget::ArenaWidget(QWidget * parent, Qt::WindowFlags flags) :
    QFrame(parent, flags),
    fmap(BSRect(10)),
    is_active(false),
    timer_id(0)
{
    QGridLayout *layout = new QGridLayout();
    layout->setContentsMargins(2, 2, 2, 2);
    layout->setSpacing(0);

    for (BSRect::iter i(fmap.get_rect()); i(); ++i) {
        ArenaCellWidget *f = new ArenaCellWidget(this);
        *fmap[*i] = f;
        layout->addWidget(f, (*i).y(), (*i).x());
        ArenaCellMediator *p = new ArenaCellMediator(f, *i);
        QObject::connect(f, SIGNAL(click()),
                         p, SLOT(click()));
        QObject::connect(p, SIGNAL(click(BSPoint)),
                         this, SLOT(accept_click(BSPoint)));
    }
    setLayout(layout);

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(4);

    setFixedSize(132, 132);

    QPalette p(palette());
    p.setColor(QPalette::WindowText, QColor(140, 140, 140));
    setPalette(p);
    activate();
}

void
ArenaWidget::timerEvent(QTimerEvent *) {
    one_step();
}

void
ArenaWidget::one_step() {
    QPalette p(palette());
    QColor c(p.color(QPalette::WindowText));
    int v(c.red());
    v = (v/20 + (is_active?-1:1))*20;
    p.setColor(QPalette::WindowText, QColor(v, v, v));
    setPalette(p);
    if ((v <= 80 || v >= 200) && timer_id != 0) {
        killTimer(timer_id);
        timer_id = 0;
    } else {
        if (timer_id == 0) {
            timer_id = startTimer(100);
        }
    }
}

void
ArenaWidget::change_color(BSPoint p, ColorStrategy c) {
    (*fmap[p])->changeColor(c);
}

void
ArenaWidget::clear() {
    ColorStrategy const c;
    for (BSRect::iter i(fmap.get_rect()); i(); ++i) {
        (*fmap[*i])->changeColor(c);
    }
}

void
ArenaWidget::activate() {
    if (! is_active) {
        is_active = true;
        setCursor(Qt::PointingHandCursor);
        one_step();
    }
}

void
ArenaWidget::deactivate() {
    if (is_active) {
        is_active = false;
        setCursor(Qt::ForbiddenCursor);
        one_step();
    }
}

// --- slots ---

void
ArenaWidget::accept_click(BSPoint p) {
    if (is_active) {
        emit click(p);
    }
}
