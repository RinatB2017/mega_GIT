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
#include "controller.h"
#include "flags_colors.h"
//#include <iostream>
#include <assert.h>

// ==============================================

const int Controller::legend[][13] = {
 {4, 3, 3, 2, 2, 2, 1, 1, 1, 1, 0},    // Russian
 {5, 4, 3, 3, 2, 0},                   // Board Game Capital

 {4, 4, 3, 3, 2, 2, 1, 0},                // Alt
 {4, 4, 4, 3, 3, 3, 0},                   // Alt
 {2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 0}, // Alt

 {10, 10, 8, 0},  // debug
 {3, 1, 0}  // debug
};

// ==============================================

Controller::Controller(QObject * parent) :
    QObject(parent),
    timer(new QTimer(this)),
    game_state(gs_edit),
    is_human_acts_first(true),
    legend_variant(0),
    arena_h_key(10, legend[legend_variant]),
    arena_h_puz(10, legend[legend_variant]),
    arena_c_key(10, legend[legend_variant]),
    arena_c_puz(10, legend[legend_variant])

{
    timer->setSingleShot(true);
    timer->setInterval(1000);
    connect(this, SIGNAL(do_battle_mode()), this, SLOT(start_battle()));
    connect(this, SIGNAL(wait_comp()), this, SLOT(start_thinking()));
    connect(timer, SIGNAL(timeout()), this, SLOT(computer_fire()));
    // restore state from QSettings
}

// ----------------------------------------------
// private slots

void
Controller::start_thinking() {
//    std::cout << "Controller::start_thinking()" << std::endl;
    assert(game_state == gs_battle);
    assert(battle_state == bs_wait_comp);
    timer->start();
}

void
Controller::computer_fire() {
//    std::cout << "Controller::computer_fire()" << std::endl;
    assert(game_state == gs_battle);
    assert(battle_state == bs_wait_comp);
    BSPoint fire = arena_c_puz.find_fire();
    BSArena::result result = arena_h_key.apply_fire(fire);
    BSRect uprect = arena_c_puz.apply_result(fire, result);
    for (BSRect::iter i(uprect); i(); ++i) {
        emit do_left_change_color(*i,
             ColorStrategy(arena_c_puz[*i], (*i == fire) ? update_mode_blink : update_mode_smooth));
    }
    switch (result) {
        case BSArena::r_milk:
            battle_state = bs_wait_human;
            emit wait_human();
            break;
        case BSArena::r_already_fired:
        case BSArena::r_wasted_effort:
            assert(false);
            break;
        case BSArena::r_continue_in_that_direction:
        case BSArena::r_drowned:
            start_thinking(); // call slot as method; not true way
            break;
        case BSArena::r_game_over:
            // теперь никто никуда не стреляет
            game_state = gs_view;
            for (BSRect::iter i(arena_c_key.get_rect()); i(); ++i) {
                 if (arena_c_key[*i].occupied() && !arena_c_key[*i].fired()) {
                     emit do_right_change_color(*i,
                     ColorStrategy(arena_c_key[*i], update_mode_blink));
                 }
            }
            emit do_view_mode();
            break;
        default:
            assert(false);
    }
}

void
Controller::start_battle() {
    arena_c_key.auto_setup();
    arena_h_puz.clean();
    arena_c_puz.clean();
    game_state = gs_battle;
    if (is_human_acts_first) {
        battle_state = bs_wait_human;
//        std::cout << "emit wait_human();" << std::endl;
        emit wait_human();
    } else {
        battle_state = bs_wait_comp;
//        std::cout << "emit wait_comp();" << std::endl;
        emit wait_comp();
    }
}

// ----------------------------------------------

void
Controller::new_game() {
    if (timer->isActive()) {
        timer->stop();
    }
    arena_h_key.clean();
    arena_c_key.clean();
    arena_h_puz.clean();
    arena_c_puz.clean();
    game_state = gs_edit;
    emit do_edit_mode();
}

void
Controller::autosetup_game() {
    assert(game_state == gs_edit);
    arena_h_key.load_legend(legend[legend_variant]);
    arena_h_key.auto_setup();
    for (BSRect::iter i(arena_h_key.get_rect()); i(); ++i) {
        emit do_left_change_color(*i,
               ColorStrategy(arena_h_key[*i], update_mode_smooth));
    }
}

void
Controller::check_and_start_game() {
    assert(game_state == gs_edit);
    arena_h_key.load_legend(legend[legend_variant]);
    arena_c_key.load_legend(legend[legend_variant]);
    arena_h_puz.load_legend(legend[legend_variant]);
    arena_c_puz.load_legend(legend[legend_variant]);
    if (arena_h_key.check()) {
        // похорошему, везде надо поступать именно так:
        // здесь только эмитировать сигнал,
        // а все действия, связанные с этим сигналом
        // надо проводить отдельно; в обработчике сигнала
        emit do_battle_mode();
    } else {
        emit alert_field_incorrect();
    }
}

void
Controller::quit() {
    // save state in QSettings
    emit do_quit();
}

void
Controller::set_rules(QAction * a) {
    legend_variant = a->actionGroup()->actions().indexOf(a);
//    std::cout << "RULES: " << legend_variant << std::endl;
    // правила вступают в силу не сейчас, а только при рестарте!
    if (game_state != gs_edit) {
        emit alert_postpone_rules();
    }
}

void
Controller::set_first_actor(QAction * a) {
    is_human_acts_first = 0 == a->actionGroup()->actions().indexOf(a);
}

void
Controller::set_speed(QAction * a) {
    static const int t[] = {500, 1000, 2000};
    timer->setInterval(t[a->actionGroup()->actions().indexOf(a)]);
}

void
Controller::left_click(BSPoint p) {
//    std::cout << "left: " << p << std::endl;
    assert(game_state == gs_edit);
    if (arena_h_key[p].occupied()) {
        arena_h_key[p].clean();
    } else {
        arena_h_key[p].occupy();
    }
    emit do_left_change_color(p, ColorStrategy(arena_h_key[p], update_mode_smooth));
}

void
Controller::right_click(BSPoint p) {
//    std::cout << "rght(1): " << p << std::endl;
    assert(game_state == gs_battle);
    assert(battle_state == bs_wait_human);
    BSArena::result r = arena_c_key.apply_fire(p);
    switch (r) {
        case BSArena::r_already_fired:
            emit alert_already_fired();
            return;
        case BSArena::r_milk:
        case BSArena::r_wasted_effort:
            // теперь стреляет компьютер
            battle_state = bs_wait_comp;
            emit wait_comp();
            break;
        case BSArena::r_continue_in_that_direction:
        case BSArena::r_drowned:
            // призовой выстрел (режим не меняется)
            break;
        case BSArena::r_game_over:
            // теперь никто никуда не стреляет
            game_state = gs_view;
            emit do_view_mode();
            break;
        default:
            assert(false);
    }
//    std::cout << "rght(2): " << p << std::endl;
    BSRect updated_area = arena_h_puz.apply_result(p, r);
//    std::cout << "rght(3): " << p << std::endl;
    for (BSRect::iter i(updated_area); i(); ++i) {
        emit do_right_change_color(*i,
             ColorStrategy(arena_h_puz[*i],
                           (*i == p) ? update_mode_blink :
                                       update_mode_smooth
                          ));
    }
}

// ----------------------------------------------

bool
Controller::connect_bulean(
    bool v,
    const QObject * sender,
    const char * signal,
    const QObject * receiver,
    const char * method)
{
    BooleanMediator * b(new BooleanMediator(this, v));
    return (
      QObject::connect(sender, signal, b, SLOT(in())) &&
      QObject::connect(b, SIGNAL(out(bool)), receiver, method)
    );
}


// ==============================================

BooleanMediator::BooleanMediator(QObject * parent, bool v) :
    QObject(parent),
    value(v)
{
}

void
BooleanMediator::in() {
    emit out(value);
}
