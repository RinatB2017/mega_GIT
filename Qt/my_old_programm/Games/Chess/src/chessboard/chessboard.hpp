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
#ifndef CHESSBOARD_HPP
#define CHESSBOARD_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QFrame>
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
class ChessBoard : public QFrame
{
    Q_OBJECT

public:
    enum Figures {
        BISHOP_BLACK = 0x1000,
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
    explicit ChessBoard(QWidget *parent = nullptr);
    bool set_figure(Figures figure, const QString &coord);
    bool get_figure(Figures *figure, const QString &coord);
    bool get_figure(Figures *figure, int x, int y);
    bool move(const QString text);
    void clear(void);

    void new_game(void);

    void test(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void message(const QString &);

private:
    QPixmap bishop_white;
    QPixmap king_white;
    QPixmap knight_white;
    QPixmap pawn_white;
    QPixmap queen_white;
    QPixmap rook_white;

    QPixmap bishop_black;
    QPixmap king_black;
    QPixmap knight_black;
    QPixmap pawn_black;
    QPixmap queen_black;
    QPixmap rook_black;

    int chessboard_map[MAX_X][MAX_Y];

    void connect_log(void);
    void init(void);

    void draw_text(QPainter *painter, const QString &text, int x, int y);
    void draw_figure(QPainter *painter, QPixmap figure, int x, int y);
    void draw_field(QPainter *painter, int x, int y, bool is_black);
    void draw_chessboard(QPainter *painter);

    QString return_figure_name(int figure);
    QString return_figure_position(int x, int y);
    bool convert_simvol_to_int(int s, int *i_s);
    bool check_coordinate(const QString &coord, int *x, int *y);

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);
};
//--------------------------------------------------------------------------------
#endif // CHESSBOARD_HPP
