/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#ifndef CHESSBOARD_HPP
#define CHESSBOARD_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QFrame>
//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class QGridLayout;
class QToolButton;
//--------------------------------------------------------------------------------
class ChessBoard : public MyWidget
{
    Q_OBJECT

public:
    enum states {
        STATE_IDLE = 0,
        STATE_1,
        STATE_2
    };
    enum Figures {
        NO_FIGURE = 0,
        BISHOP_BLACK,
        KING_BLACK,
        KNIGHT_BLACK,
        PAWN_BLACK,
        QUEEN_BLACK,
        ROOK_BLACK,
        BISHOP_WHITE,
        KING_WHITE,
        KNIGHT_WHITE,
        PAWN_WHITE,
        QUEEN_WHITE,
        ROOK_WHITE
    };
    ChessBoard(QWidget *parent = nullptr);
    bool set_figure(Figures figure, const QString &coord);
    bool set_figure(Figures figure, int x, int y);

    bool get_figure(Figures *figure, const QString &coord);
    bool get_figure(Figures *figure, int x, int y);

    bool move(const QString text);

signals:
    void s_move(QString data);

public slots:
    void new_game(void);

private slots:
    void set_cursor(void);
    void click(void);

private:
    QPixmap w_bishop;
    QPixmap w_king;
    QPixmap w_knight;
    QPixmap w_pawn;
    QPixmap w_queen;
    QPixmap w_rook;

    QPixmap b_bishop;
    QPixmap b_king;
    QPixmap b_knight;
    QPixmap b_pawn;
    QPixmap b_queen;
    QPixmap b_rook;

    QGridLayout *chessboard_grid;
    QToolButton *btn_chessboard[8][8];

    QString first_str;
    QString second_str;

    int state = STATE_1;

    void init(void);

    void create_chessboard(void);
    void create_figures(void);

    QString return_figure_name(int figure);
    QString return_figure_position(int x, int y);

    bool convert_simvol_to_int(int s, int *i_s);
    bool check_coordinate(const QString &coord, int *x, int *y);

    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif // CHESSBOARD_HPP
