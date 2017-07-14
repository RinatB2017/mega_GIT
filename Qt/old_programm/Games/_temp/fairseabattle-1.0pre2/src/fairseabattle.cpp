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
#include <QApplication>
#include <QBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QIcon>
#include <QToolBar>
#include <QMainWindow>
#include "alerter.h"
#include "arenawidget.h"
#include "controller.h"
//#include "battleframe.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
    Q_INIT_RESOURCE(fairseabattle);

    QApplication app(argc, argv);

    // создаём окно приложения
    QMainWindow main_window;
    main_window.setWindowTitle(QObject::tr("Fair Sea Battle"));
    main_window.layout()->setSizeConstraint(QLayout::SetFixedSize);

    // создаём контроллер
    Controller * ctrl = new Controller(&main_window);

    // создаём и связываем нтификатор
    Alerter * alert = new Alerter(&main_window);

    QObject::connect(ctrl, SIGNAL(alert_field_incorrect()), alert, SLOT(invalid_field_setup()));
    QObject::connect(ctrl, SIGNAL(alert_postpone_rules()), alert, SLOT(postpone_rules()));
    QObject::connect(ctrl, SIGNAL(alert_already_fired()), alert, SLOT(field_already_fired()));

    // создаём арены
    ArenaWidget * left_arena = new ArenaWidget;
    ArenaWidget * right_arena = new ArenaWidget;

    QObject::connect(left_arena, SIGNAL(click(BSPoint)), ctrl, SLOT(left_click(BSPoint)));
    QObject::connect(ctrl, SIGNAL(do_left_change_color(BSPoint, ColorStrategy)), left_arena, SLOT(change_color(BSPoint, ColorStrategy)));
    QObject::connect(ctrl, SIGNAL(do_right_change_color(BSPoint, ColorStrategy)), right_arena, SLOT(change_color(BSPoint, ColorStrategy)));
    QObject::connect(right_arena, SIGNAL(click(BSPoint)), ctrl, SLOT(right_click(BSPoint)));

    // связываем арены
    // переходя в режим редактирования переактивируем поля и отчищаем их
    QObject::connect(ctrl, SIGNAL(do_edit_mode()), left_arena, SLOT(activate()));
    QObject::connect(ctrl, SIGNAL(do_edit_mode()), right_arena, SLOT(deactivate()));
    QObject::connect(ctrl, SIGNAL(do_edit_mode()), left_arena, SLOT(clear()));
    QObject::connect(ctrl, SIGNAL(do_edit_mode()), right_arena, SLOT(clear()));
    // при начале баталии гасим одно поле
    QObject::connect(ctrl, SIGNAL(do_battle_mode()), left_arena, SLOT(deactivate()));
    // we do not care about right field on do_battle_mode()
    // а вот при стрельбе активизируем правое
    QObject::connect(ctrl, SIGNAL(wait_human()), right_arena, SLOT(activate()));
    QObject::connect(ctrl, SIGNAL(wait_comp()), right_arena, SLOT(deactivate()));
    // на всякий случай деактивируем правое, левое и так не активно
    QObject::connect(ctrl, SIGNAL(do_view_mode()), right_arena, SLOT(deactivate()));

    // размещаем арены
    QFrame * central_widget = new QFrame();
    QBoxLayout * layout = new QBoxLayout(QBoxLayout::LeftToRight);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    layout->addWidget(left_arena);
    layout->addWidget(right_arena);
    central_widget->setLayout(layout);
    main_window.setCentralWidget(central_widget);

//    BattleFrame *battle = new BattleFrame();
//    main_window.setCentralWidget(battle);

    // созадём меню
    BooleanMediator *med;
    QAction *a;
    QMenu *m;
    QMenuBar *mbar(main_window.menuBar());
    QToolBar *t = new QToolBar();
    QActionGroup *ag;

    m = mbar->addMenu(QObject::tr("Game"));

    // начальное положение тулбара надо брать
    // из ctrl (когда оно там появится :-))
    main_window.addToolBar(Qt::BottomToolBarArea, t);
    t->setAllowedAreas(Qt::AllToolBarAreas);

    a = new QAction(QIcon(":/images/new.png"), QObject::tr("New game"), &main_window);
//    a->setIconVisibleInMenu(false);
    a->setShortcut(Qt::CTRL+Qt::Key_N);
    QObject::connect(a, SIGNAL(triggered()), ctrl, SLOT(new_game()));
    ctrl->connect_bulean(true, ctrl, SIGNAL(do_battle_mode()), a, SLOT(setEnabled(bool)));
    ctrl->connect_bulean(false, ctrl, SIGNAL(do_edit_mode()), a, SLOT(setEnabled(bool)));
    m->addAction(a);
    t->addAction(a);

    a = new QAction(QIcon(":/images/auto.png"), QObject::tr("Auto setup game"), &main_window);
    a->setShortcut(Qt::CTRL+Qt::Key_X);
    QObject::connect(a, SIGNAL(triggered()), ctrl, SLOT(autosetup_game()));
    med = new BooleanMediator(ctrl, false);
    QObject::connect(ctrl, SIGNAL(do_battle_mode()), med, SLOT(in()));
    QObject::connect(med, SIGNAL(out(bool)), a, SLOT(setEnabled(bool)));
    med = new BooleanMediator(ctrl, true);
    QObject::connect(ctrl, SIGNAL(do_edit_mode()), med, SLOT(in()));
    QObject::connect(med, SIGNAL(out(bool)), a, SLOT(setEnabled(bool)));
    m->addAction(a);
    t->addAction(a);

    a = new QAction(QIcon(":/images/go.png"), QObject::tr("Check and start game"), &main_window);
    a->setShortcut(Qt::CTRL+Qt::Key_C);
    QObject::connect(a, SIGNAL(triggered()), ctrl, SLOT(check_and_start_game()));
    med = new BooleanMediator(ctrl, false);
    QObject::connect(ctrl, SIGNAL(do_battle_mode()), med, SLOT(in()));
    QObject::connect(med, SIGNAL(out(bool)), a, SLOT(setEnabled(bool)));
    med = new BooleanMediator(ctrl, true);
    QObject::connect(ctrl, SIGNAL(do_edit_mode()), med, SLOT(in()));
    QObject::connect(med, SIGNAL(out(bool)), a, SLOT(setEnabled(bool)));
    m->addAction(a);
    t->addAction(a);

    m->addSeparator();

    a = new QAction(QObject::tr("Quit"), &main_window);
    a->setShortcut(Qt::CTRL+Qt::Key_Q);
    QObject::connect(a, SIGNAL(triggered()), ctrl, SLOT(quit()));
    m->addAction(a);

//

    m = mbar->addMenu(QObject::tr("Rules"));
    ag = new QActionGroup(&main_window);
    QObject::connect(ag, SIGNAL(triggered(QAction *)),
                     ctrl, SLOT(set_rules(QAction *)));
    a = new QAction(QObject::tr("Classic (4+3+3+2+2+2+1+1+1+1)"), &main_window);
    a->setShortcut(Qt::CTRL+Qt::Key_1);
    a->setCheckable(true);
    a->setChecked(true); // <-- load forom config?
    ag->addAction(a);
    m->addAction(a);
    a = new QAction(QObject::tr("Salvo variant (5+4+3+3+2)"), &main_window);
    a->setShortcut(Qt::CTRL+Qt::Key_2);
    a->setCheckable(true);
    ag->addAction(a);
    m->addAction(a);

    a = new QAction(QObject::tr("Longship alernative (4+4+3+3+2+2+1)"), &main_window);
    a->setShortcut(Qt::CTRL+Qt::Key_3);
    a->setCheckable(true);
    ag->addAction(a);
    m->addAction(a);
    a = new QAction(QObject::tr("Balanced alernative (4+4+4+3+3+3)"), &main_window);
    a->setShortcut(Qt::CTRL+Qt::Key_4);
    a->setCheckable(true);
    ag->addAction(a);
    m->addAction(a);
    a = new QAction(QObject::tr("Shortship alernative (2x8+1x4)"), &main_window);
    a->setShortcut(Qt::CTRL+Qt::Key_5);
    a->setCheckable(true);
    ag->addAction(a);
    m->addAction(a);

    a = new QAction(QObject::tr("Debug-1 (10+10+8)"), &main_window);
    a->setShortcut(Qt::CTRL+Qt::Key_6);
    a->setCheckable(true);
    ag->addAction(a);
    m->addAction(a);
    a = new QAction(QObject::tr("Debug-2 (3+1)"), &main_window);
    a->setShortcut(Qt::CTRL+Qt::Key_7);
    a->setCheckable(true);
    ag->addAction(a);
    m->addAction(a);

    m->addSeparator();

    ag = new QActionGroup(&main_window);
    QObject::connect(ag, SIGNAL(triggered(QAction *)),
                     ctrl, SLOT(set_first_actor(QAction *)));
    a = new QAction(QObject::tr("Human first"), &main_window);
    a->setShortcut(Qt::CTRL+Qt::Key_H);
    a->setCheckable(true);
    a->setChecked(true); // <-- load forom config?
    ag->addAction(a);
    m->addAction(a);
    a = new QAction(QObject::tr("Computer First"), &main_window);
    a->setShortcut(Qt::CTRL+Qt::Key_M);
    a->setCheckable(true);
    ag->addAction(a);
    m->addAction(a);

    m->addSeparator();

    ag = new QActionGroup(&main_window);
    QObject::connect(ag, SIGNAL(triggered(QAction *)),
                     ctrl, SLOT(set_speed(QAction *)));
    a = new QAction(QObject::tr("Fast"), &main_window);
    a->setShortcut(Qt::CTRL+Qt::Key_A);
    a->setCheckable(true);
    ag->addAction(a);
    m->addAction(a);
    a = new QAction(QObject::tr("Normal speed"), &main_window);
    a->setShortcut(Qt::CTRL+Qt::Key_B);
    a->setCheckable(true);
    a->setChecked(true); // <-- load forom config?
    ag->addAction(a);
    m->addAction(a);
    a = new QAction(QObject::tr("Slow"), &main_window);
    a->setShortcut(Qt::CTRL+Qt::Key_D);
    a->setCheckable(true);
    ag->addAction(a);
    m->addAction(a);

//

    m = mbar->addMenu(QObject::tr("Help"));

    a = new QAction(QObject::tr("Help"), &main_window);
    a->setShortcut(Qt::Key_F1);
    QObject::connect(a, SIGNAL(triggered()), alert, SLOT(help()));
    m->addAction(a);
    a = new QAction(QObject::tr("About"), &main_window);
    QObject::connect(a, SIGNAL(triggered()), alert, SLOT(about()));
    m->addAction(a);
    m->addSeparator();
    a = new QAction(QObject::tr("About Qt"), &main_window);
    QObject::connect(a, SIGNAL(triggered()), alert, SLOT(about_qt()));
    m->addAction(a);

    // связываем меню

    // связки
    QObject::connect(ctrl, SIGNAL(do_quit()), &app, SLOT(quit()));

    // emit new_game
    ctrl->new_game();

    //
    main_window.show();

    //sranddev(); // mingw do not support it
    srand(time(0));

    return app.exec();
}
