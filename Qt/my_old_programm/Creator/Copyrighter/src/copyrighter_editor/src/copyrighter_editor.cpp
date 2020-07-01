/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#include "copyrighter_editor.hpp"
#include "spellcheckdialog.hpp"
#include "spellchecker.hpp"

#include "ui_copyrighter_editor.h"
//--------------------------------------------------------------------------------
Copyrighter_editor::Copyrighter_editor(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Copyrighter_editor)
{
    ui->setupUi(this);

    init();
}
//--------------------------------------------------------------------------------
Copyrighter_editor::~Copyrighter_editor()
{
    delete ui;
}
//--------------------------------------------------------------------------------
QString Copyrighter_editor::toPlainText(void)
{
    return ui->editor_widget->toPlainText();
}
//--------------------------------------------------------------------------------
void Copyrighter_editor::append(const QString &text)
{
    ui->editor_widget->append(text);
}
//--------------------------------------------------------------------------------
void Copyrighter_editor::init(void)
{
    connect(ui->btn_check,      &QPushButton::clicked,      this,   &Copyrighter_editor::check);
    connect(ui->editor_widget,  &MyTextEdit::textChanged,   this,   &Copyrighter_editor::show_info);
    connect(ui->editor_widget,  &MyTextEdit::textChanged,   this,   &Copyrighter_editor::check_it);
    check();
    show_info();
}
//--------------------------------------------------------------------------------
void Copyrighter_editor::check_it(void)
{
    if(ui->cb_auto->isChecked())
    {
        check();
    }
}
//--------------------------------------------------------------------------------
void Copyrighter_editor::check(void)
{
    QString dictPath = "/usr/share/hunspell/ru_RU";
    QString userDict = "/usr/share/hunspell/ru_RU.aff";
    SpellChecker *spellChecker = new SpellChecker(dictPath, userDict);

    QTextCharFormat highlightFormat;
#if 0
    highlightFormat.setBackground(QBrush(QColor("#ff6060")));
    highlightFormat.setForeground(QBrush(QColor("#000000")));
#else
    // alternative format
    highlightFormat.setUnderlineColor(QColor("red"));
    highlightFormat.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
#endif

    // save the position of the current cursor
    QTextCursor oldCursor = ui->editor_widget->textCursor();

    // create a new cursor to walk through the text
    QTextCursor cursor(ui->editor_widget->document());

    // Don't call cursor.beginEditBlock(), as this prevents the rewdraw after changes to the content
    // cursor.beginEditBlock();

    //TODO перенёс сюда
    QList<QTextEdit::ExtraSelection> esList;

    while(!cursor.atEnd())
    {
        QCoreApplication::processEvents();
        cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor, 1);
        QString word = cursor.selectedText();

        // Workaround for better recognition of words
        // punctuation etc. does not belong to words
        while(!word.isEmpty() && !word.at(0).isLetter() && cursor.anchor() < cursor.position())
        {
            int cursorPos = cursor.position();
            cursor.setPosition(cursor.anchor() + 1, QTextCursor::MoveAnchor);
            cursor.setPosition(cursorPos, QTextCursor::KeepAnchor);
            word = cursor.selectedText();
        }

        if(!word.isEmpty() && !spellChecker->spell(word))
        {
            QTextCursor tmpCursor(cursor);
            tmpCursor.setPosition(cursor.anchor());
            ui->editor_widget->setTextCursor(tmpCursor);
            ui->editor_widget->ensureCursorVisible();

            // highlight the unknown word
            QTextEdit::ExtraSelection es;
            es.cursor = cursor;
            es.format = highlightFormat;

            //TODO перенёс отсюда
            //QList<QTextEdit::ExtraSelection> esList;

            esList << es;
            ui->editor_widget->setExtraSelections(esList);
            QCoreApplication::processEvents();
        }
        cursor.movePosition(QTextCursor::NextWord, QTextCursor::MoveAnchor, 1);
    }
    //cursor.endEditBlock();
    ui->editor_widget->setTextCursor(oldCursor);
}
//--------------------------------------------------------------------------------
void Copyrighter_editor::check_orig(void)
{
    QString dictPath = "/path/to/your/dictionary/de_DE_neu/de_DE_neu";
    QString userDict= "/tmp/userDict.txt";
    SpellChecker *spellChecker = new SpellChecker(dictPath, userDict);
    SpellCheckDialog *checkDialog = new SpellCheckDialog(spellChecker, this);

    QTextCharFormat highlightFormat;
    highlightFormat.setBackground(QBrush(QColor("#ff6060")));
    highlightFormat.setForeground(QBrush(QColor("#000000")));
    // alternative format
    //highlightFormat.setUnderlineColor(QColor("red"));
    //highlightFormat.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);

    // save the position of the current cursor
    QTextCursor oldCursor = ui->editor_widget->textCursor();

    // create a new cursor to walk through the text
    QTextCursor cursor(ui->editor_widget->document());

    // Don't call cursor.beginEditBlock(), as this prevents the rewdraw after changes to the content
    // cursor.beginEditBlock();
    while(!cursor.atEnd()) {
        QCoreApplication::processEvents();
        cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor, 1);
        QString word = cursor.selectedText();

        // Workaround for better recognition of words
        // punctuation etc. does not belong to words
        while(!word.isEmpty() && !word.at(0).isLetter() && cursor.anchor() < cursor.position()) {
            int cursorPos = cursor.position();
            cursor.setPosition(cursor.anchor() + 1, QTextCursor::MoveAnchor);
            cursor.setPosition(cursorPos, QTextCursor::KeepAnchor);
            word = cursor.selectedText();
        }

        if(!word.isEmpty() && !spellChecker->spell(word)) {
            QTextCursor tmpCursor(cursor);
            tmpCursor.setPosition(cursor.anchor());
            ui->editor_widget->setTextCursor(tmpCursor);
            ui->editor_widget->ensureCursorVisible();

            // highlight the unknown word
            QTextEdit::ExtraSelection es;
            es.cursor = cursor;
            es.format = highlightFormat;

            QList<QTextEdit::ExtraSelection> esList;
            esList << es;
            ui->editor_widget->setExtraSelections(esList);
            QCoreApplication::processEvents();

            // ask the user what to do
            SpellCheckDialog::SpellCheckAction spellResult = checkDialog->checkWord(word);

            // reset the word highlight
            esList.clear();
            ui->editor_widget->setExtraSelections(esList);
            QCoreApplication::processEvents();

            if(spellResult == SpellCheckDialog::AbortCheck)
                break;

            switch(spellResult) {
            case SpellCheckDialog::ReplaceOnce:
                cursor.insertText(checkDialog->replacement());
                break;

            default:
                break;
            }
            QCoreApplication::processEvents();
        }
        cursor.movePosition(QTextCursor::NextWord, QTextCursor::MoveAnchor, 1);
    }
    //cursor.endEditBlock();
    ui->editor_widget->setTextCursor(oldCursor);

    QMessageBox::information(
                this,
                tr("Finished"),
                tr("The spell check has finished."));
}
//--------------------------------------------------------------------------------
void Copyrighter_editor::show_info(void)
{
    QString clean_src = ui->editor_widget->toPlainText()
            .remove(" ")
            .remove("\"")
            .remove("(")
            .remove(")")
            .remove(".")
            .remove(",")
            .remove("\r")
            .remove("\n")
            .remove("\t");

    ui->lbl_info->setText(QString(tr("text %1 clean text %2"))
                          .arg(ui->editor_widget->toPlainText().size())
                          .arg(clean_src.length()));

}
//--------------------------------------------------------------------------------
void Copyrighter_editor::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool Copyrighter_editor::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Copyrighter_editor::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void Copyrighter_editor::save_setting(void)
{

}
//--------------------------------------------------------------------------------
