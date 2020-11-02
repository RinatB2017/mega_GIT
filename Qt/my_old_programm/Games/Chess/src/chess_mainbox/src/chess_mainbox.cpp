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
#include "ui_chess_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "defines.hpp"
#include "chess_mainbox.hpp"
//--------------------------------------------------------------------------------
#include "chessboard.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    if(m_engine)
    {
        m_engine->kill();
        m_engine->waitForFinished();
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif
    createChessBar();
    create_chessboard();
    create_engine();
    new_game();

    connect(this,   &MainBox::invalide_move,    this,   &MainBox::restore_move);

    adjustSize();
    setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
void MainBox::createChessBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *chessbar = new QToolBar("chessbar");
    chessbar->setObjectName("chessbar");
    mw->addToolBar(Qt::TopToolBarArea, chessbar);

    le_test = new QLineEdit(this);
    le_test->setFixedWidth(100);
    le_test->setText("e2e4");
    chessbar->addWidget(le_test);

    btn_run = add_button(chessbar,
                         new QToolButton(this),
                         qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                         "run",
                         "run");

    connect(btn_run,    &QToolButton::clicked,      this,   &MainBox::run);
    connect(le_test,    &QLineEdit::returnPressed,  this,   &MainBox::run);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    btn_test = add_button(testbar,
                          new QToolButton(this),
                          qApp->style()->standardIcon(QStyle::SP_CommandLink),
                          "test",
                          "test");

    connect(btn_test,   &QToolButton::clicked,  this,   &MainBox::test);
}
//--------------------------------------------------------------------------------
void MainBox::create_chessboard(void)
{
    connect(ui->chessboard_widget,  &ChessBoard::s_move,    this,   &MainBox::move);
}
//--------------------------------------------------------------------------------
void MainBox::move(QString text)
{
    emit debug(QString("move(%1)").arg(text));
    text += "\n";
    m_engine->write(text.toLocal8Bit());
}
//--------------------------------------------------------------------------------
void MainBox::create_engine(void)
{
    m_engine = new QProcess(this);
    m_engine->setProcessChannelMode(QProcess::SeparateChannels);
    //m_engine->setOpenMode(QProcess::ReadWrite);
    //m_engine->setReadChannel(QProcess::StandardOutput);

    connect(m_engine,   &QProcess::readyReadStandardOutput, this,   &MainBox::readData);
    connect(m_engine,   &QProcess::readyReadStandardError,  this,   &MainBox::readError);

    m_engine->setProgram("gnuchess");
    m_engine->start();
    m_pid = m_engine->pid();

#if 1
    //TODO
    m_engine->write("xboard\n");
    m_engine->write("protover 2\n");    //новая версия протокола
#endif

    whiteMoveRegEx    = QRegExp("\\. \\b([a-h][1-8][a-h][1-8])(q|r|b|n|)\\b");
    blackMoveRegEx    = QRegExp("\\. \\.\\.\\. \\b([a-h][1-8][a-h][1-8])(q|r|b|n|)\\b");
    illegalMoveRegEx  = QRegExp("Illegal move(?::| \\(([a-zA-Z0-9\\s]+)\\) :) (\\S+)");
    invalidMoveRegEx  = QRegExp("Invalid move(?::| \\(([a-zA-Z0-9\\s]+)\\) :) (\\S+)");
    resultOutputRegEx = QRegExp("(0-1|1-0|1/2-1/2) \\{([a-zA-Z0-9\\s]+)\\}");
    coordenateRegEx   = QRegExp("\\b([a-h][1-8][a-h][1-8])(q|r|b|n|)\\b");
}
//--------------------------------------------------------------------------------
//void MainBox::x1(void)
//{
//    emit info(m_engine->readAll());
//}
//--------------------------------------------------------------------------------
//void MainBox::x2(void)
//{
//    emit info(m_engine->readAll());
//}
//--------------------------------------------------------------------------------
void MainBox::new_game(void)
{
    ui->chessboard_widget->new_game();
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    ui->chessboard_widget->clear_figures();
    ui->chessboard_widget->set_figure(ChessBoard::KING_BLACK, "A1");
    ui->chessboard_widget->set_figure(ChessBoard::KING_BLACK, "B2");
    ui->chessboard_widget->set_figure(ChessBoard::KING_BLACK, "C3");
    ui->chessboard_widget->set_figure(ChessBoard::KING_BLACK, "D4");
    ui->chessboard_widget->set_figure(ChessBoard::KING_BLACK, "E5");
    ui->chessboard_widget->set_figure(ChessBoard::KING_BLACK, "F6");
    ui->chessboard_widget->set_figure(ChessBoard::KING_BLACK, "G7");
    ui->chessboard_widget->set_figure(ChessBoard::KING_BLACK, "H8");
}
//--------------------------------------------------------------------------------
void MainBox::run(void)
{
    QString text = le_test->text().trimmed();
    le_test->clear();

    if(text.isEmpty())
    {
        emit error("text empty");
        return;
    }

    text.append("\n");
    if(m_engine)
    {
        m_engine->write(text.toLatin1());
    }
}
//--------------------------------------------------------------------------------
bool MainBox::analize(const QString &line)
{
    if(illegalMoveRegEx.indexIn(line) >= 0)
    {
        emit debug(QString("illegalMoveRegEx: %1").arg(illegalMoveRegEx.cap(0)));
        if(coordenateRegEx.indexIn(line) >= 0)
        {
            emit error(QString("coordenateRegEx: %1").arg(coordenateRegEx.cap(1)));
            return false;
        }
    }

    if(invalidMoveRegEx.indexIn(line) >= 0)
    {
        emit debug(QString("invalidMoveRegEx: %1").arg(invalidMoveRegEx.cap(0)));
        if(coordenateRegEx.indexIn(line) >= 0)
        {
            emit error(QString("Неверный ход: %1").arg(coordenateRegEx.cap(1)));
            return false;
        }
    }

    if(resultOutputRegEx.indexIn(line) >= 0)
    {
        emit error(QString("resultOutputRegEx: %1").arg(resultOutputRegEx.cap(1)));
        return false;
    }

    //---

    if(blackMoveRegEx.indexIn(line) >= 0)
    {
        emit info("Ваш ход");
        emit debug(QString("blackMoveRegEx: %1").arg(blackMoveRegEx.cap(1)));
        ui->chessboard_widget->move(blackMoveRegEx.cap(1));
        return true;
    }

    if(whiteMoveRegEx.indexIn(line) >= 0)
    {
        emit info("Ход противника");
        emit debug(QString("whiteMoveRegEx: %1").arg(whiteMoveRegEx.cap(1)));
        ui->chessboard_widget->move(whiteMoveRegEx.cap(1));
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
void MainBox::readData(void)
{
    QString output = m_engine->readAllStandardOutput();
    if(output.isEmpty())
    {
        return;
    }
    QStringList lines = output.split("\n");
    emit debug(QString("received %1 bytes").arg(output.size()));
    emit debug(QString("lines.size() %1").arg(lines.size()));
    for(int n=0; n<lines.size(); n++)
    {
        QString line = lines.at(n);
        emit trace(QString("%1").arg(line));
        analize(line);
    }
}
//--------------------------------------------------------------------------------
void MainBox::readError(void)
{
    QString output = m_engine->readAllStandardError();
    if(output.isEmpty())
    {
        return;
    }
    emit error(output);
}
//--------------------------------------------------------------------------------
void MainBox::restore_move(void)
{
    emit error("restore_move");
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
