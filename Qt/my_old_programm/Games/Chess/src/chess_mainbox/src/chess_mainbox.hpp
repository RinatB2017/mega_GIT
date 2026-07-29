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
#include <QPointer>
#include <QProcess>
#include <QRegExp>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class ChessBoard;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    virtual ~MainBox();

signals:
    void invalide_move();

private slots:
    void test();
    void run();
//    void x1();
//    void x2();
    void reverse_chessboard();
    void readData();
    void readError();
    void new_game();

    void restore_move();
    void move(QString text);

private:
    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;

    QProcess *m_engine = nullptr;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    Q_PID m_pid;
#endif

    void init();

    QPointer<QLineEdit>     le_chess;
    QPointer<QComboBox>     cb_test;

    QPointer<QToolButton>   btn_clear;
    QPointer<QToolButton>   btn_test;
    QPointer<QToolButton>   btn_run;

    QRegExp whiteMoveRegEx;
    QRegExp blackMoveRegEx;
    QRegExp illegalMoveRegEx;
    QRegExp invalidMoveRegEx;
    QRegExp resultOutputRegEx;
    QRegExp coordenateRegEx;

    bool analize(const QString &line);

    void createChessBar();
    void createTestBar();
    void create_chessboard();
    void create_engine();

    void updateText();
    bool programm_is_exit();
    void load_setting();
    void save_setting();
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
