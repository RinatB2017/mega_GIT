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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#if defined(Q_OS_WIN)
#   include "Qsci/qsciscintilla.h"
#   include "Qsci/qscilexercpp.h"
#   include "Qsci/qscilexerjavascript.h"
#else
#   include <Qsci/qsciscintilla.h>
#   include <Qsci/qscilexercpp.h>
#   include <Qsci/qscilexerjavascript.h>
#endif
//--------------------------------------------------------------------------------
#include "ui_qscintilla_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "qscintilla_mainbox.hpp"
#include "defines.hpp"
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
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifndef QT_DEBUG
    Q_CHECK_PTR(parentWidget());
#endif

    createTestBar();

    QsciLexerCPP *lexCpp = new QsciLexerCPP(this);
    ui->te_CPP->setUtf8(true);
    ui->te_CPP->setLexer(lexCpp);

    QsciLexerJavaScript *lexJS = new QsciLexerJavaScript(this);
    ui->te_JS->setUtf8(true);
    ui->te_JS->setLexer(lexJS);

    //---
    //! Текущая строка кода и ее подсветка
    ui->te_CPP->setCaretLineVisible(true);
    ui->te_CPP->setCaretLineBackgroundColor(QColor("gainsboro"));

    //! Выравнивание
    ui->te_CPP->setAutoIndent(true);
    ui->te_CPP->setIndentationGuides(false);
    ui->te_CPP->setIndentationsUseTabs(true);
    ui->te_CPP->setIndentationWidth(4);

    //! margin это полоска слева, на которой обычно распологаются breakpoints
    ui->te_CPP->setMarginsBackgroundColor(QColor("gainsboro"));
    ui->te_CPP->setMarginLineNumbers(1, true);
    ui->te_CPP->setMarginWidth(1, 50);

    //! Авто-дополнение кода в зависимости от источника
    ui->te_CPP->setAutoCompletionSource(QsciScintilla::AcsAll);
    ui->te_CPP->setAutoCompletionCaseSensitivity(true);
    ui->te_CPP->setAutoCompletionReplaceWord(true);
    ui->te_CPP->setAutoCompletionUseSingle(QsciScintilla::AcusAlways);
    ui->te_CPP->setAutoCompletionThreshold(0);

    //! Подсветка соответствий скобок
    ui->te_CPP->setBraceMatching(QsciScintilla::SloppyBraceMatch);
    ui->te_CPP->setMatchedBraceBackgroundColor(Qt::yellow);
    ui->te_CPP->setUnmatchedBraceForegroundColor(Qt::blue);
    //---
    //! margin это полоска слева, на которой обычно распологаются breakpoints
    ui->te_JS->setMarginsBackgroundColor(QColor("gainsboro"));
    ui->te_JS->setMarginLineNumbers(1, true);
    ui->te_JS->setMarginWidth(1, 50);
    //---

    QString temp_CPP;
    temp_CPP.append("class MainBox : public QWidget\n");
    temp_CPP.append("{\n");
    temp_CPP.append("   Q_OBJECT\n");
    temp_CPP.append("\n");
    temp_CPP.append("   public:\n");
    temp_CPP.append("       explicit MainBox(QWidget *parent);\n");
    temp_CPP.append("       ~MainBox();\n");
    temp_CPP.append("\n");
    temp_CPP.append("   private:\n");
    temp_CPP.append("       void init(void);\n");
    temp_CPP.append("       void function(void)\n");
    temp_CPP.append("       {\n");
    temp_CPP.append("           int x = 5;\n");
    temp_CPP.append("       }\n");
    temp_CPP.append("};\n");

    QString temp_JS;
    temp_JS.append("function()\n");
    temp_JS.append("{\n");
    temp_JS.append("   alert(document.title)\n");
    temp_JS.append("}\n");

    ui->te_CPP->setText(temp_CPP);
    ui->te_JS->setText(temp_JS);

#ifdef Q_OS_LINUX
    //TODO белый цвет, если тема темная
    //ui->textEdit->setStyleSheet("background:white;");
#endif

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test 0", &MainBox::test_0 });
    commands.append({ id++, "test 1", &MainBox::test_1 });
    commands.append({ id++, "test 2", &MainBox::test_2 });
    commands.append({ id++, "test 3", &MainBox::test_3 });
    commands.append({ id++, "test 4", &MainBox::test_4 });
    commands.append({ id++, "test 5", &MainBox::test_5 });
    commands.append({ id++, "test 6", nullptr });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_block = new QCheckBox("block", this);
    testbar->addWidget(cb_block);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    connect(cb_block, SIGNAL(clicked(bool)), cb_test,           SLOT(setDisabled(bool)));
    connect(cb_block, SIGNAL(clicked(bool)), btn_choice_test,   SLOT(setDisabled(bool)));
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok)
    {
        return;
    }
    auto cmd_it = std::find_if(
        commands.begin(),
        commands.end(),
        [cmd](CMD command){ return command.cmd == cmd; }
    );
    if (cmd_it != commands.end())
    {
        typedef bool (MainBox::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (this->*x)();
        }
        else
        {
            emit error("no func");
        }
    }
}
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit info("Test_0()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_1()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit info("Test_2()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit info("Test_3()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit info("Test_4()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit info("Test_5()");
    return true;
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
