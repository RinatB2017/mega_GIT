#include "debugoutput.h"
#include "ui_debugoutput.h"
#include <QPlainTextEdit>

DebugOutput::DebugOutput(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebugOutput)
{
    ui->setupUi(this);
    this->ptr_to_txtfield = NULL;

}

DebugOutput::~DebugOutput()
{
    delete ui;
}

void DebugOutput::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void DebugOutput::Output(QString str)
{
    if(ptr_to_txtfield == NULL)
    {

         ptr_to_txtfield = this->findChild<QPlainTextEdit*>("OutputTextField");
    }

    ptr_to_txtfield->appendPlainText(str);






}
