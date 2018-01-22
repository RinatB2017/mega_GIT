/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QProcess>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class QToolButton;
class QLineEdit;
class QToolBar;
class QLabel;

class ChessBoard;
class QProcess;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent,
            MySplashScreen *splash);
    ~MainBox();

signals:
    void invalide_move(void);

private slots:
    void test(void);
    void run(void);
    void x1(void);
    void x2(void);
    void readData(void);
    void new_game(void);

    void restore_move(void);
    void move(QString text);

private:
    MySplashScreen *splash = 0;
    Ui::MainBox *ui = 0;

#ifndef NO_CHESSBOARD
    ChessBoard *board = 0;
#endif
    QProcess   *m_engine = 0;
    Q_PID m_pid;

    void init(void);

    QLineEdit   *le_test = 0;

    QToolButton *btn_test = 0;
    QToolButton *btn_run = 0;

    QRegExp whiteMoveRegEx;
    QRegExp blackMoveRegEx;
    QRegExp illegalMoveRegEx;
    QRegExp invalidMoveRegEx;
    QRegExp resultOutputRegEx;
    QRegExp coordenateRegEx;

    bool analize(const QString &line);

    void createTestBar(void);
    void create_chessboard(void);
    void create_engine(void);

    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
