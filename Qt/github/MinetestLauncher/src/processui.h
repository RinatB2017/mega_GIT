#ifndef PROCESSUI_H
#define PROCESSUI_H

#include <QProcess>
#include <QTextEdit>
#include <QString>

class ProcessUi : public QTextEdit
{
    Q_OBJECT
public:
    ProcessUi(QWidget*parent, QString cmd);
    void start();

private slots:
    void readStdOutput();
    void readError();

private:
    QString command;
    QProcess* process;
};

#endif // PROCESSUI_H
