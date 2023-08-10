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
#include "ui_qscintilla_mainbox.h"
//--------------------------------------------------------------------------------
#include "qscintilla_mainbox.hpp"
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

#ifdef QT_DEBUG
    createTestBar();
#endif

    QsciLexerCPP *lexCpp = new QsciLexerCPP(this);
    ui->te_CPP->setUtf8(true);
    ui->te_CPP->setLexer(lexCpp);

    QsciLexerJavaScript *lexJS = new QsciLexerJavaScript(this);
    ui->te_JS->setUtf8(true);
    ui->te_JS->setLexer(lexJS);

    QsciLexerCSS *lexCSS = new QsciLexerCSS(this);
    ui->te_CSS->setUtf8(true);
    ui->te_CSS->setLexer(lexCSS);

    QsciLexerCSharp *lexCSharp = new QsciLexerCSharp(this);
    ui->te_CSharp->setUtf8(true);
    ui->te_CSharp->setLexer(lexCSharp);

    QsciLexerHTML *lexHTML = new QsciLexerHTML(this);
    ui->te_HTML->setUtf8(true);
    ui->te_HTML->setLexer(lexHTML);

    QsciLexerJava *lexJava = new QsciLexerJava(this);
    ui->te_Java->setUtf8(true);
    ui->te_Java->setLexer(lexJava);

    QsciLexerPython *lexPython = new QsciLexerPython(this);
    ui->te_Python->setUtf8(true);
    ui->te_Python->setLexer(lexPython);

    QsciLexerPascal *lexPascal = new QsciLexerPascal(this);
    ui->te_Pascal->setUtf8(true);
    ui->te_Pascal->setLexer(lexPascal);

    QList<QsciScintilla *> l_lexer = findChildren<QsciScintilla *>();
    emit debug(QString("cnt = %1").arg(l_lexer.count()));
    foreach (QsciScintilla *lexer, l_lexer)
    {
        // Текущая строка кода и ее подсветка
        lexer->setCaretLineVisible(true);
        lexer->setCaretLineBackgroundColor(QColor("gainsboro"));

        // Выравнивание
        lexer->setAutoIndent(true);
        lexer->setIndentationGuides(false);
        lexer->setIndentationsUseTabs(true);
        lexer->setIndentationWidth(4);

        // margin это полоска слева, на которой обычно располагаются breakpoints
        lexer->setMarginsBackgroundColor(QColor("gainsboro"));
        lexer->setMarginLineNumbers(1, true);
        lexer->setMarginWidth(1, 50);

        // Авто-дополнение кода в зависимости от источника
        lexer->setAutoCompletionSource(QsciScintilla::AcsAll);
        lexer->setAutoCompletionCaseSensitivity(true);
        lexer->setAutoCompletionReplaceWord(true);
        lexer->setAutoCompletionUseSingle(QsciScintilla::AcusAlways);
        lexer->setAutoCompletionThreshold(0);

        // Подсветка соответствий скобок
        lexer->setBraceMatching(QsciScintilla::SloppyBraceMatch);
        lexer->setMatchedBraceBackgroundColor(Qt::yellow);
        lexer->setUnmatchedBraceForegroundColor(Qt::blue);
    }

    bool ok;
    QString cpp_data;
    QString js_data;
    QString css_data;
    QString cs_data;
    QString html_data;
    QString java_data;
    QString py_data;
    QString pas_data;

    ok = read_file(":/data/data.cpp", &cpp_data);
    if(ok)  ui->te_CPP->setText(cpp_data);

    ok = read_file(":/data/data.js", &js_data);
    if(ok) ui->te_JS->setText(js_data);

    ok = read_file(":/data/data.css", &css_data);
    if(ok) ui->te_CSS->setText(css_data);

    ok = read_file(":/data/data.cs", &cs_data);
    if(ok) ui->te_CSharp->setText(cs_data);

    ok = read_file(":/data/data.html", &html_data);
    if(ok) ui->te_HTML->setText(html_data);

    ok = read_file(":/data/data.java", &java_data);
    if(ok) ui->te_Java->setText(java_data);

    ok = read_file(":/data/data.py", &py_data);
    if(ok) ui->te_Python->setText(py_data);

    ok = read_file(":/data/data.pas", &pas_data);
    if(ok) ui->te_Pascal->setText(pas_data);

#ifdef Q_OS_LINUX
    //TODO белый цвет, если тема темная
    //ui->textEdit->setStyleSheet("background:white;");
#endif

    load_widgets();
}
//--------------------------------------------------------------------------------
bool MainBox::read_file(const QString &filename, QString *data)
{
    Q_ASSERT(data);
    if(filename.isEmpty())
    {
        emit error("filename is empty!");
        return false;
    }

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        emit error(QString("File %1 not read").arg(filename));
        return false;
    }
    QByteArray ba = file.readAll();
    *data = ba.data();

    file.close();

    return true;
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    if(mw)
    {
        commands.clear(); int id = 0;
        commands.append({ id++, "test", &MainBox::test });

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
    else
    {
        emit error("mw not found!");
    }
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
bool MainBox::test(void)
{
    emit info("Test()");
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
