#include "breakpoint_form.h"
#include "ui_breakpoint_form.h"

Breakpoint_Form::Breakpoint_Form(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::Breakpoint_Form)
{
    ui->setupUi(this);
}

Breakpoint_Form::~Breakpoint_Form()
{
    delete ui;
}

void Breakpoint_Form::changeEvent(QEvent *e)
{
    QGroupBox::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
