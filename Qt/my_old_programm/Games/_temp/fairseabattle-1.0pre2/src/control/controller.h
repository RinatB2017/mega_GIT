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
#ifndef BS_CONTROL_CONTROLLER_H
#define BS_CONTROL_CONTROLLER_H

#include <QObject>
#include <QAction>
#include <QTimer>
#include "geometry.h"
#include "arenawidget.h"
#include "arena.h"

// ===================================================

/* ���������� �������� ������� �� ��������� ����������.
   ��� ������� �������������� �� ����� �����:
   -> ���������� �������� ������� ������������ ��������
      (�������� ����� ���� �����������)
   -> ���������� ��������� ����������� ��������� �����������
 ???> ��������� ������� ������ �������
      (������ _������_ �������; ����, �� ������� ���� ���������
      ��������� ���������� ��������, �� ����� ������ �������
      ��������� _������������_ ������ �� ��������� ������)
*/


class BooleanMediator;


class Controller : public QObject {

    Q_OBJECT

private:
    enum GameState {
        gs_edit,     // ��������������
        gs_battle,   // �����
        gs_view      // ��������
    };
    enum BattleState {
        bs_wait_human, // �ģ� ������� �������
        bs_wait_comp   // �ģ� ������� ����������
    };

    QTimer * timer;

    GameState game_state;
    BattleState battle_state;

    bool          is_human_acts_first;
    int           legend_variant;

    BSArenaKey    arena_h_key;
    BSArenaPuzzle arena_h_puz;
    BSArenaKey    arena_c_key;
    BSArenaPuzzle arena_c_puz;

    static const int legend[][13];

public:
    Controller(QObject * parent = 0);
    bool connect_bulean(
        bool v,
        const QObject * sender,
        const char * signal,
        const QObject * receiver,
        const char * method);

private slots:
    void start_battle();
    void start_thinking();
    void computer_fire();

public slots:
    void quit();
    void new_game();
    void autosetup_game();
    void check_and_start_game();
    void set_rules(QAction *);
    void set_first_actor(QAction *);
    void set_speed(QAction *);
    void left_click(BSPoint);
    void right_click(BSPoint);

signals:
    void do_quit(); // ��������� ���������
    void do_edit_mode(); // ������ ��������������
    void do_battle_mode(); // ������ �����
    void do_view_mode(); // �������� ������ ����������� ���������

    void wait_human(); // �ģ� ���� �� ��������
    void wait_comp();  // �ģ� ���� �� ����������

    void do_left_change_color(BSPoint, ColorStrategy); // �������� ����
    void do_right_change_color(BSPoint, ColorStrategy); // �������� ����

                                  // ��������� �...
    void alert_field_incorrect(); //  ����������� �� �����
    void alert_postpone_rules();  //  ������� ������� � ���� �����
    void alert_already_fired();   //  ��� ��������
};

// ===================================================

class BooleanMediator : public QObject {

    Q_OBJECT

private:
    bool value;

public:
    BooleanMediator(QObject * parent = 0, bool value = false);

public slots:
    void in();

signals:
    void out(bool);
};


#endif
