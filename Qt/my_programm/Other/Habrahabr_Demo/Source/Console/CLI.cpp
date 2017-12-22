#include "CLI.h"

#include <QString>
#include <QtGui>
#include <QScrollBar>
#include <QPalette>

CLI::CLI(QWidget* parent) : QTextEdit(parent)
{
    promt = "Habrahabr> ";

    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::green);
    setPalette(p);

    insertPromt(false);
    isLocked = false;

    time = new QTime();

    history = new QStringList();
    historyPos = 0;
}

void CLI::keyPressEvent(QKeyEvent* event)
{
    if (isLocked)
    {
        return;
    }

    // При вводе обычных букв, цифр и других полезных символов, они должны добавляться в строку команды
    if (event->key() >= 0x20 && event->key() <= 0x7e
           && (event->modifiers() == Qt::NoModifier || event->modifiers() == Qt::ShiftModifier))
    {
            QTextEdit::keyPressEvent(event);
    }


   // Символы можно стирать клавишей Backspace, но не все, а только до
   //     определенного момента – чтобы строка приглашения не затерлась
    if (event->key() == Qt::Key_Backspace
           && event->modifiers() == Qt::NoModifier
           && textCursor().positionInBlock() > promt.length())
    {
            QTextEdit::keyPressEvent(event);
    }

    // реакция виджета на ввод команды (при нажатии клавиши Enter)
    if (event->key() == Qt::Key_Return && event->modifiers() == Qt::NoModifier)
    {
            onEnter();
    }

    // Перемещение по истории команд клавишами "Вверх" и "Вниз"
    if (event->key() == Qt::Key_Up && event->modifiers() == Qt::NoModifier)
    {
        historyBack();
    }

    if (event->key() == Qt::Key_Down && event->modifiers() == Qt::NoModifier)
    {
        historyForward();
    }

}

void CLI::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(*event)
}

void CLI::contextmenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(*event)
}

void CLI::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(*event)
    // При клике мышкой, делаем консоль активной
    setFocus();
}

void CLI::insertPromt(bool insertNewBlock)
{
    scrollDown();

    QTextCharFormat format;
    format.setForeground(Qt::green);
    textCursor().setBlockCharFormat(format);

    if(insertNewBlock)
    {
        textCursor().insertBlock();
    }
    textCursor().insertText(promt);
}

/*
При вводе команды мы вырезаем кусок текста от строки приглашения до конца текстового блока и
испускаем сигнал, к которому можно будет присоединить слот
*/
void CLI::onEnter()
{
    // На время обработки команды, фиксируем консоль от изменений
    isLocked = true;

    if (textCursor().positionInBlock() == promt.length())
    {
        insertPromt();
        return;
    }

    QString cmd = textCursor().block().text().mid(promt.length());

    historyAdd(cmd);
    emit onCommand(cmd);

    insertPromt();
    isLocked = false;
}

void CLI::output(QString s)
{
    QTextCharFormat format;
    QString a;

    //format.setForeground(Qt::white);
    textCursor().setBlockCharFormat(format);
    textCursor().insertBlock();
    a = time->currentTime().toString() +":  " + s;
    textCursor().insertText(a);

    // Разблокировка консоли
    isLocked = false;

    // Вывод приглашения к вводу
    insertPromt();
}

void CLI::scrollDown()
{
    QScrollBar *vbar = verticalScrollBar();
    vbar->setValue(vbar->maximum());
}

void CLI::historyAdd(QString cmd)
{
    history->append(cmd);

    historyPos = history->length();
}

void CLI::historyBack()
{
    if (!historyPos)
    {
        return;
    }

    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.insertText(promt + history->at(historyPos - 1));
    setTextCursor(cursor);
    historyPos--;
}

void CLI::historyForward()
{
    if (historyPos == history->length())
    {
            return;
    }

    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();

    if (historyPos == history->length() - 1)
    {
        cursor.insertText(promt);
    }
    else
    {
        cursor.insertText(promt + history->at(historyPos + 1));
    }

    setTextCursor(cursor);
    historyPos++;
}
