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
#include <QToolButton>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "chessboard.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
ChessBoard::ChessBoard(QWidget *parent) :
    MyWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
void ChessBoard::create_figures(void)
{
    b_bishop.load(BISHOP_BLACK_PIC);
    b_king.load(KING_BLACK_PIC);
    b_knight.load(KNIGHT_BLACK_PIC);
    b_pawn.load(PAWN_BLACK_PIC);
    b_queen.load(QUEEN_BLACK_PIC);
    b_rook.load(ROOK_BLACK_PIC);

    w_bishop.load(BISHOP_WHITE_PIC);
    w_king.load(KING_WHITE_PIC);
    w_knight.load(KNIGHT_WHITE_PIC);
    w_pawn.load(PAWN_WHITE_PIC);
    w_queen.load(QUEEN_WHITE_PIC);
    w_rook.load(ROOK_WHITE_PIC);
}
//--------------------------------------------------------------------------------
void ChessBoard::create_chessboard(void)
{
    chessboard_grid = new QGridLayout();

    chessboard_grid->setMargin(0);
    chessboard_grid->setSpacing(0);

    QFont font;
    font.setBold(true);
    bool b = false;
    for(int y=0; y<8; y++)
    {
        for(int x=0; x<8; x++)
        {
            btn_chessboard[x][y] = new QToolButton(this);
            btn_chessboard[x][y]->setIconSize(QSize(WIDTH_FIELD, HEIGHT_FIELD));
            btn_chessboard[x][y]->setCheckable(true);
            btn_chessboard[x][y]->setIcon(QIcon());
            connect(btn_chessboard[x][y],   SIGNAL(clicked(bool)),  this,   SLOT(click(bool)));

            if(b)
                btn_chessboard[x][y]->setStyleSheet("background:gray;");
            else
                btn_chessboard[x][y]->setStyleSheet("background:white;");

            connect(btn_chessboard[x][y], SIGNAL(clicked(bool)), this, SLOT(set_cursor()));

            chessboard_grid->addWidget(btn_chessboard[x][y], y, x);
            b=!b;
        }

        QLabel *l_temp = new QLabel(QString("%1").arg(abs(y-8)));
        l_temp->setAlignment(Qt::AlignLeft);
        l_temp->setFont(font);
        chessboard_grid->addWidget(l_temp, y, 9);
        b=!b;
    }
    QLabel *l_A = new QLabel("A");  l_A->setAlignment(Qt::AlignHCenter); l_A->setFont(font);
    QLabel *l_B = new QLabel("B");  l_B->setAlignment(Qt::AlignHCenter); l_B->setFont(font);
    QLabel *l_C = new QLabel("C");  l_C->setAlignment(Qt::AlignHCenter); l_C->setFont(font);
    QLabel *l_D = new QLabel("D");  l_D->setAlignment(Qt::AlignHCenter); l_D->setFont(font);
    QLabel *l_E = new QLabel("E");  l_E->setAlignment(Qt::AlignHCenter); l_E->setFont(font);
    QLabel *l_F = new QLabel("F");  l_F->setAlignment(Qt::AlignHCenter); l_F->setFont(font);
    QLabel *l_G = new QLabel("G");  l_G->setAlignment(Qt::AlignHCenter); l_G->setFont(font);
    QLabel *l_H = new QLabel("H");  l_H->setAlignment(Qt::AlignHCenter); l_H->setFont(font);

    chessboard_grid->addWidget(l_A, 9, 0);
    chessboard_grid->addWidget(l_B, 9, 1);
    chessboard_grid->addWidget(l_C, 9, 2);
    chessboard_grid->addWidget(l_D, 9, 3);
    chessboard_grid->addWidget(l_E, 9, 4);
    chessboard_grid->addWidget(l_F, 9, 5);
    chessboard_grid->addWidget(l_G, 9, 6);
    chessboard_grid->addWidget(l_H, 9, 7);
}
//--------------------------------------------------------------------------------
void ChessBoard::new_game(void)
{
    for(int y=0; y<MAX_Y; y++)
    {
        for(int x=0; x<MAX_X; x++)
        {
            set_figure(NO_FIGURE, x, y);
        }
    }

    set_figure(ChessBoard::ROOK_WHITE,   "A1");
    set_figure(ChessBoard::KNIGHT_WHITE, "B1");
    set_figure(ChessBoard::BISHOP_WHITE, "C1");
    set_figure(ChessBoard::QUEEN_WHITE,  "D1");
    set_figure(ChessBoard::KING_WHITE,   "E1");
    set_figure(ChessBoard::BISHOP_WHITE, "F1");
    set_figure(ChessBoard::KNIGHT_WHITE, "G1");
    set_figure(ChessBoard::ROOK_WHITE,   "H1");

    set_figure(ChessBoard::PAWN_WHITE, "A2");
    set_figure(ChessBoard::PAWN_WHITE, "B2");
    set_figure(ChessBoard::PAWN_WHITE, "C2");
    set_figure(ChessBoard::PAWN_WHITE, "D2");
    set_figure(ChessBoard::PAWN_WHITE, "E2");
    set_figure(ChessBoard::PAWN_WHITE, "F2");
    set_figure(ChessBoard::PAWN_WHITE, "G2");
    set_figure(ChessBoard::PAWN_WHITE, "H2");

    set_figure(ChessBoard::ROOK_BLACK,   "A8");
    set_figure(ChessBoard::KNIGHT_BLACK, "B8");
    set_figure(ChessBoard::BISHOP_BLACK, "C8");
    set_figure(ChessBoard::QUEEN_BLACK,  "D8");
    set_figure(ChessBoard::KING_BLACK,   "E8");
    set_figure(ChessBoard::BISHOP_BLACK, "F8");
    set_figure(ChessBoard::KNIGHT_BLACK, "G8");
    set_figure(ChessBoard::ROOK_BLACK,   "H8");

    set_figure(ChessBoard::PAWN_BLACK, "A7");
    set_figure(ChessBoard::PAWN_BLACK, "B7");
    set_figure(ChessBoard::PAWN_BLACK, "C7");
    set_figure(ChessBoard::PAWN_BLACK, "D7");
    set_figure(ChessBoard::PAWN_BLACK, "E7");
    set_figure(ChessBoard::PAWN_BLACK, "F7");
    set_figure(ChessBoard::PAWN_BLACK, "G7");
    set_figure(ChessBoard::PAWN_BLACK, "H7");
}
//--------------------------------------------------------------------------------
void ChessBoard::init(void)
{
    create_figures();
    create_chessboard();
    new_game();

    setLayout(chessboard_grid);
    setFixedSize(WIDTH_FIELD * (MAX_X + 2), HEIGHT_FIELD * (MAX_Y + 2));
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
bool ChessBoard::convert_simvol_to_int(int s, int *i_s)
{
    bool ok = true;
    switch(s)
    {
    case '1': *i_s=7; break;
    case '2': *i_s=6; break;
    case '3': *i_s=5; break;
    case '4': *i_s=4; break;
    case '5': *i_s=3; break;
    case '6': *i_s=2; break;
    case '7': *i_s=1; break;
    case '8': *i_s=0; break;

    case 'A': *i_s=0; break;
    case 'B': *i_s=1; break;
    case 'C': *i_s=2; break;
    case 'D': *i_s=3; break;
    case 'E': *i_s=4; break;
    case 'F': *i_s=5; break;
    case 'G': *i_s=6; break;
    case 'H': *i_s=7; break;

    default:
        ok = false;
        break;
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

    emit debug(QString("set_figure: figure %1 coord %2 ").arg(return_figure_name(figure)).arg(coord));

    switch(figure)
    {
    case NO_FIGURE:     btn_chessboard[x][y]->setIcon(QIcon());  break;

    case BISHOP_BLACK:  btn_chessboard[x][y]->setIcon(b_bishop); break;
    case KING_BLACK:    btn_chessboard[x][y]->setIcon(b_king);   break;
    case KNIGHT_BLACK:  btn_chessboard[x][y]->setIcon(b_knight); break;
    case PAWN_BLACK:    btn_chessboard[x][y]->setIcon(b_pawn);   break;
    case QUEEN_BLACK:   btn_chessboard[x][y]->setIcon(b_queen);  break;
    case ROOK_BLACK:    btn_chessboard[x][y]->setIcon(b_rook);   break;

    case BISHOP_WHITE:  btn_chessboard[x][y]->setIcon(w_bishop); break;
    case KING_WHITE:    btn_chessboard[x][y]->setIcon(w_king);   break;
    case KNIGHT_WHITE:  btn_chessboard[x][y]->setIcon(w_knight); break;
    case PAWN_WHITE:    btn_chessboard[x][y]->setIcon(w_pawn);   break;
    case QUEEN_WHITE:   btn_chessboard[x][y]->setIcon(w_queen);  break;
    case ROOK_WHITE:    btn_chessboard[x][y]->setIcon(w_rook);   break;
    }
    btn_chessboard[x][y]->setProperty("figure_name", figure);

    return ok;
}
//--------------------------------------------------------------------------------
bool ChessBoard::set_figure(Figures figure, int x, int y)
{
    //King(король), Queen(феерзь), Rook(ладья), Knight(конь), Bishop(слон), Pawn(пешка)

    if(x<0) return false;
    if(x>MAX_X) return false;
    if(y<0) return false;
    if(y>MAX_X) return false;

    emit debug(QString("figure %1").arg(figure));

    switch(figure)
    {
    case NO_FIGURE:     btn_chessboard[x][y]->setIcon(QIcon());     break;
    case BISHOP_BLACK:  btn_chessboard[x][y]->setIcon(b_bishop);    break;
    case KING_BLACK:    btn_chessboard[x][y]->setIcon(b_king);      break;
    case KNIGHT_BLACK:  btn_chessboard[x][y]->setIcon(b_knight);    break;
    case PAWN_BLACK:    btn_chessboard[x][y]->setIcon(b_pawn);      break;
    case QUEEN_BLACK:   btn_chessboard[x][y]->setIcon(b_queen);     break;
    case ROOK_BLACK:    btn_chessboard[x][y]->setIcon(b_rook);      break;
    case BISHOP_WHITE:  btn_chessboard[x][y]->setIcon(w_bishop);    break;
    case KING_WHITE:    btn_chessboard[x][y]->setIcon(w_king);      break;
    case KNIGHT_WHITE:  btn_chessboard[x][y]->setIcon(w_knight);    break;
    case PAWN_WHITE:    btn_chessboard[x][y]->setIcon(w_pawn);      break;
    case QUEEN_WHITE:   btn_chessboard[x][y]->setIcon(w_queen);     break;
    case ROOK_WHITE:    btn_chessboard[x][y]->setIcon(w_rook);      break;
    }
    btn_chessboard[x][y]->setProperty("figure_name", figure);

    return true;
}
//--------------------------------------------------------------------------------
bool ChessBoard::get_figure(Figures *figure, const QString &coord)
{
    //King(король), Queen(феерзь), Rook(ладья), Knight(конь), Bishop(слон), Pawn(пешка)

    int x = 0;
    int y = 0;
    bool ok = check_coordinate(coord, &x, &y);
    if(!ok) return false;

    *figure = (Figures)btn_chessboard[x][y]->property("figure_name").toInt();
    return true;
}
//--------------------------------------------------------------------------------
bool ChessBoard::get_figure(Figures *figure, int x, int y)
{
    //King(король), Queen(феерзь), Rook(ладья), Knight(конь), Bishop(слон), Pawn(пешка)

    *figure = (Figures)btn_chessboard[x][y]->property("figure_name").toInt();
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

    set_figure(NO_FIGURE, from);
    set_figure(figure_from, to);

    emit info(QString("[%1 %2] [%3 %4] %5")
              .arg(x1)
              .arg(y1)
              .arg(x2)
              .arg(y2)
              .arg(return_figure_name(figure_from)));
    return true;
}
//--------------------------------------------------------------------------------
void ChessBoard::set_cursor(void)
{
    QToolButton *btn = (QToolButton *)sender();
    Q_CHECK_PTR(btn);
    if(!btn)
    {
        emit error("set_cursor");
        return;
    }

    QPixmap pixmap = btn->icon().pixmap(WIDTH_FIELD, HEIGHT_FIELD);
    //id = btn->property("property_id").toInt();
    if(!pixmap.isNull())
    {
        setCursor(QCursor(pixmap));
    }
}
//--------------------------------------------------------------------------------
void ChessBoard::click(bool state)
{
    emit debug(QString("state is %1").arg(state));
}
//--------------------------------------------------------------------------------
void ChessBoard::updateText(void)
{

}
//--------------------------------------------------------------------------------
