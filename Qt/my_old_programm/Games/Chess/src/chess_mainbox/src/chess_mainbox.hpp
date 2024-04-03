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
    void invalide_move(void);

private slots:
    void test(void);
    void run(void);
//    void x1(void);
//    void x2(void);
    void reverse_chessboard(void);
    void readData(void);
    void readError(void);
    void new_game(void);

    void restore_move(void);
    void move(QString text);

private:
    QPointer<MySplashScreen> splash;
    Ui::MainBox *ui;

    QProcess *m_engine = nullptr;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    Q_PID m_pid;
#endif

    void init(void);

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

    void createChessBar(void);
    void createTestBar(void);
    void create_chessboard(void);
    void create_engine(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
