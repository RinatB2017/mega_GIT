#ifndef CLI_H
#define CLI_H

#include <QString>
#include <QTime>
#include <QTextEdit>

class CLI : public QTextEdit
{
    Q_OBJECT

public:
    explicit CLI(QWidget* parent = 0);

    void output(QString s);
    void scrollDown();

protected:
    void keyPressEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void contextmenuEvent(QContextMenuEvent* event);

private:
    QString promt;              // Приглашение к вводу
    QTime* time;
    bool isLocked;
    QStringList* history;       // История введенных команд
    int historyPos;


    void insertPromt(bool insertNewBlock = true); // Вывод строки приглашения
    void onEnter(void);             // Обработка команды

    void historyBack();             // Перемещение назад по истории команд
    void historyForward();          // Перемещение вперед по истории команд
    void historyAdd(QString cmd);   // Добавляем введенную команду в список введенных команд

signals:
    void onCommand(QString cmd);
};

#endif // CLI_H
