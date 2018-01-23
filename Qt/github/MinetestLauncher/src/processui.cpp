#include "processui.h"

ProcessUi::ProcessUi(QWidget *parent, QString cmd)
    : QTextEdit(parent)
{
    command = cmd;
    setReadOnly(true);

    process = new QProcess(this);
    connect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(readStdOutput()));
    connect(process,SIGNAL(error(QProcess::ProcessError)),this,SLOT(readError()));
}

void ProcessUi::start()
{
    append("Executing: " + command);
    process->start(command);
}

void ProcessUi::readStdOutput()
{
    append(process->readAllStandardOutput());
}

void ProcessUi::readError()
{
    append("An Error Occured! Error Code is: " + QString::number(process->error()));
}
