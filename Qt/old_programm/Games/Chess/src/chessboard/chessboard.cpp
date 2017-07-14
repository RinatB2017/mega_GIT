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
#include <QMouseEvent>
#include <QPainter>
#include <QFrame>
#include <QBrush>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "chessboard.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
ChessBoard::ChessBoard(QWidget *parent) :
    QFrame(parent)
{
    init();
}
//--------------------------------------------------------------------------------
void ChessBoard::connect_log(void)
{
    connect(this, SIGNAL(info(QString)),  topLevelWidget(), SIGNAL(info(QString)));
    connect(this, SIGNAL(debug(QString)), topLevelWidget(), SIGNAL(debug(QString)));
    connect(this, SIGNAL(error(QString)), topLevelWidget(), SIGNAL(error(QString)));
}
//--------------------------------------------------------------------------------
void ChessBoard::init(void)
{
    connect_log();

    bishop_white = QPixmap(BISHOP_WHITE_PIC);
    king_white   = QPixmap(KING_WHITE_PIC);
    knight_white = QPixmap(KNIGHT_WHITE_PIC);
    pawn_white   = QPixmap(PAWN_WHITE_PIC);
    queen_white  = QPixmap(QUEEN_WHITE_PIC);
    rook_white   = QPixmap(ROOK_WHITE_PIC);

    bishop_black = QPixmap(BISHOP_BLACK_PIC);
    king_black   = QPixmap(KING_BLACK_PIC);
    knight_black = QPixmap(KNIGHT_BLACK_PIC);
    pawn_black   = QPixmap(PAWN_BLACK_PIC);
    queen_black  = QPixmap(QUEEN_BLACK_PIC);
    rook_black   = QPixmap(ROOK_BLACK_PIC);

    clear();

    setFixedSize(WIDTH_FIELD * (MAX_X + 2), HEIGHT_FIELD * (MAX_Y + 2));
    setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
}
//--------------------------------------------------------------------------------
void ChessBoard::draw_field(QPainter *painter, int x, int y, bool is_black)
{
    painter->save();
    if(is_black)
        painter->setBrush(QBrush(QColor(Qt::gray)));
    else
        painter->setBrush(QBrush(QColor(Qt::white)));
    painter->drawRect(x*WIDTH_FIELD, y*HEIGHT_FIELD, WIDTH_FIELD, HEIGHT_FIELD);
    painter->restore();
}
//--------------------------------------------------------------------------------
void ChessBoard::draw_figure(QPainter *painter, QPixmap figure, int x, int y)
{
    x++;
    y++;
    painter->save();
    painter->drawPixmap(QRect(x*WIDTH_FIELD, y*HEIGHT_FIELD, WIDTH_FIELD, HEIGHT_FIELD), figure);
    painter->restore();
}
//--------------------------------------------------------------------------------
void ChessBoard::draw_text(QPainter *painter, const QString &text, int x, int y)
{
    painter->save();
    painter->drawText(x*WIDTH_FIELD, y*HEIGHT_FIELD, WIDTH_FIELD, HEIGHT_FIELD, Qt::AlignCenter, text);
    painter->restore();
}
//--------------------------------------------------------------------------------
void ChessBoard::draw_chessboard(QPainter *painter)
{
    bool b = true;
    for(int y=1; y<=8; y++)
    {
        for(int x=1; x<=8; x++)
        {
            b = !b;
            draw_field(painter, x, y, b);
        }
        b = !b;
    }
}
//--------------------------------------------------------------------------------
void ChessBoard::mousePressEvent(QMouseEvent *event)
{
    int x = 0;
    int y = 0;
    switch(event->button())
    {
    case Qt::LeftButton:
        x = event->x() / WIDTH_FIELD;
        y = event->y() / HEIGHT_FIELD;
        if((x>0 && x<9) && (y>0 && y<9))
        {
            emit info(QString("%1 %2 %3 %4")
                      .arg(x)
                      .arg(y)
                      .arg(return_figure_position(x, y))
                      .arg(return_figure_name(chessboard_map[x][y])));
        }
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
QString ChessBoard::return_figure_name(int figure)
{
    QString temp;
    switch(figure)
    {
    case BISHOP_BLACK:  temp = "bishop black"; break;
    case KING_BLACK:    temp = "king black"; break;
    case KNIGHT_BLACK:  temp = "knight black"; break;
    case PAWN_BLACK:    temp = "pawn black"; break;
    case QUEEN_BLACK:   temp = "queen black"; break;
    case ROOK_BLACK:    temp = "rook black"; break;
    case BISHOP_WHITE:  temp = "bishop white"; break;
    case KING_WHITE:    temp = "king white"; break;
    case KNIGHT_WHITE:  temp = "knight white"; break;
    case PAWN_WHITE:    temp = "pawn white"; break;
    case QUEEN_WHITE:   temp = "queen white"; break;
    case ROOK_WHITE:    temp = "rook white"; break;
    default:            temp = "no figure"; break;
    }
    return temp;
}
//--------------------------------------------------------------------------------
QString ChessBoard::return_figure_position(int x, int y)
{
    QString temp_x;
    QString temp_y;
    switch(x)
    {
    case 1: temp_x="A"; break;
    case 2: temp_x="B"; break;
    case 3: temp_x="C"; break;
    case 4: temp_x="D"; break;
    case 5: temp_x="E"; break;
    case 6: temp_x="F"; break;
    case 7: temp_x="G"; break;
    case 8: temp_x="H"; break;
    }
    temp_y = QString::number(y);

    return temp_x+temp_y;
}
//--------------------------------------------------------------------------------
void ChessBoard::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    draw_chessboard(&painter);

    draw_text(&painter, "8", 0, 1);
    draw_text(&painter, "7", 0, 2);
    draw_text(&painter, "6", 0, 3);
    draw_text(&painter, "5", 0, 4);
    draw_text(&painter, "4", 0, 5);
    draw_text(&painter, "3", 0, 6);
    draw_text(&painter, "2", 0, 7);
    draw_text(&painter, "1", 0, 8);

    draw_text(&painter, "8", 9, 1);
    draw_text(&painter, "7", 9, 2);
    draw_text(&painter, "6", 9, 3);
    draw_text(&painter, "5", 9, 4);
    draw_text(&painter, "4", 9, 5);
    draw_text(&painter, "3", 9, 6);
    draw_text(&painter, "2", 9, 7);
    draw_text(&painter, "1", 9, 8);

    draw_text(&painter, "a", 1, 0);
    draw_text(&painter, "b", 2, 0);
    draw_text(&painter, "c", 3, 0);
    draw_text(&painter, "d", 4, 0);
    draw_text(&painter, "e", 5, 0);
    draw_text(&painter, "f", 6, 0);
    draw_text(&painter, "g", 7, 0);
    draw_text(&painter, "h", 8, 0);

    draw_text(&painter, "a", 1, 9);
    draw_text(&painter, "b", 2, 9);
    draw_text(&painter, "c", 3, 9);
    draw_text(&painter, "d", 4, 9);
    draw_text(&painter, "e", 5, 9);
    draw_text(&painter, "f", 6, 9);
    draw_text(&painter, "g", 7, 9);
    draw_text(&painter, "h", 8, 9);

    for(int y=0; y<MAX_Y; y++)
    {
        for(int x=0; x<MAX_X; x++)
        {
            int figure = chessboard_map[x][y];
            if(figure)
            {
                switch(figure)
                {
                case BISHOP_BLACK: draw_figure(&painter, bishop_black, x, abs(y-7)); break;
                case KING_BLACK:   draw_figure(&painter, king_black,   x, abs(y-7)); break;
                case KNIGHT_BLACK: draw_figure(&painter, knight_black, x, abs(y-7)); break;
                case PAWN_BLACK:   draw_figure(&painter, pawn_black,   x, abs(y-7)); break;
                case QUEEN_BLACK:  draw_figure(&painter, queen_black,  x, abs(y-7)); break;
                case ROOK_BLACK:   draw_figure(&painter, rook_black,   x, abs(y-7)); break;
                case BISHOP_WHITE: draw_figure(&painter, bishop_white, x, abs(y-7)); break;
                case KING_WHITE:   draw_figure(&painter, king_white,   x, abs(y-7)); break;
                case KNIGHT_WHITE: draw_figure(&painter, knight_white, x, abs(y-7)); break;
                case PAWN_WHITE:   draw_figure(&painter, pawn_white,   x, abs(y-7)); break;
                case QUEEN_WHITE:  draw_figure(&painter, queen_white,  x, abs(y-7)); break;
                case ROOK_WHITE:   draw_figure(&painter, rook_white,   x, abs(y-7)); break;
                default: break;
                }
            }
        }
    }
}
//--------------------------------------------------------------------------------
bool ChessBoard::convert_simvol_to_int(int s, int *i_s)
{
    bool ok = true;
    switch(s)
    {
    case '1': *i_s=0; break;
    case '2': *i_s=1; break;
    case '3': *i_s=2; break;
    case '4': *i_s=3; break;
    case '5': *i_s=4; break;
    case '6': *i_s=5; break;
    case '7': *i_s=6; break;
    case '8': *i_s=7; break;

    case 'A': *i_s=0; break;
    case 'B': *i_s=1; break;
    case 'C': *i_s=2; break;
    case 'D': *i_s=3; break;
    case 'E': *i_s=4; break;
    case 'F': *i_s=5; break;
    case 'G': *i_s=6; break;
    case 'H': *i_s=7; break;
    default: ok = false;
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool ChessBoard::check_coordinate(const QString &coord, int *x, int *y)
{
    if(coord.size() != 2) return false;

    bool ok = false;
    int temp_x = 0;
    int temp_y = 0;

    ok = convert_simvol_to_int(coord.at(0).toUpper().toLatin1(), &temp_x);
    if(!ok) return false;
    ok = convert_simvol_to_int(coord.at(1).toUpper().toLatin1(), &temp_y);
    if(!ok) return false;

    if(temp_x<0)
    {
        emit debug("temp_x<0");
        return false;
    }
    if(temp_y<0)
    {
        emit debug("temp_y<0");
        return false;
    }

    if(temp_x>9)
    {
        emit debug("temp_x>9");
        return false;
    }
    if(temp_y>9)
    {
        emit debug("temp_y>9");
        return false;
    }

    *x = temp_x;
    *y = temp_y;

    return true;
}
//--------------------------------------------------------------------------------
bool ChessBoard::set_figure(Figures figure, const QString &coord)
{
    //King(король), Queen(феерзь), Rook(ладья), Knight(конь), Bishop(слон), Pawn(пешка)

    int x = 0;
    int y = 0;
    bool ok = check_coordinate(coord, &x, &y);
    if(!ok) return false;

    chessboard_map[x][y] = figure;
    return ok;
}
//--------------------------------------------------------------------------------
bool ChessBoard::get_figure(Figures *figure, const QString &coord)
{
    //King(король), Queen(феерзь), Rook(ладья), Knight(конь), Bishop(слон), Pawn(пешка)

    int x = 0;
    int y = 0;
    bool ok = check_coordinate(coord, &x, &y);
    if(!ok) return false;

    *figure = (Figures)chessboard_map[x][y];
    return true;
}
//--------------------------------------------------------------------------------
bool ChessBoard::get_figure(Figures *figure, int x, int y)
{
    //King(король), Queen(феерзь), Rook(ладья), Knight(конь), Bishop(слон), Pawn(пешка)

    if(!chessboard_map[x][y])
    {
        return false;
    }

    *figure = (Figures)chessboard_map[x][y];
    return true;
}
//--------------------------------------------------------------------------------
bool ChessBoard::move(const QString text)
{
    if(text.length() != 4)
    {
        emit error("size != 4");
        return false;
    }

    QString from = text.left(2);
    QString to = text.right(2);

    emit info(QString("move: form %1 to %2").arg(from).arg(to));

    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    bool ok = false;
    ok = check_coordinate(from, &x1, &y1);
    if(!ok)
    {
        emit error(QString("check_coordinate: from [%1]").arg(from));
        return false;
    }
    ok = check_coordinate(to, &x2, &y2);
    if(!ok)
    {
        emit error(QString("check_coordinate: to [%1]").arg(to));
        return false;
    }

    Figures figure_from;
    ok = get_figure(&figure_from, x1, y1);
    if(!ok)
    {
        emit error(QString("get_figure: move from x1 [%1] y1 [%2]").arg(x1).arg(y1));
        return false;
    }

    chessboard_map[x1][y1] = 0;
    chessboard_map[x2][y2] = figure_from;
    repaint();

    emit info(QString("[%1 %2] [%3 %4] %5")
              .arg(x1)
              .arg(y1)
              .arg(x2)
              .arg(y2)
              .arg(return_figure_name(figure_from)));
    return true;
}
//--------------------------------------------------------------------------------
void ChessBoard::clear(void)
{
    for(int y=0; y<MAX_Y; y++)
    {
        for(int x=0; x<MAX_X; x++)
        {
            chessboard_map[x][y] = 0;
        }
    }
}
//--------------------------------------------------------------------------------
void ChessBoard::test(void)
{
    for(int y=0; y<MAX_Y; y++)
    {
        QString temp;
        temp.clear();
        for(int x=0; x<MAX_X; x++)
        {
            temp.append(QString("%1 ").arg(chessboard_map[x][y]));
        }
        emit info(temp);
    }
}
//--------------------------------------------------------------------------------
