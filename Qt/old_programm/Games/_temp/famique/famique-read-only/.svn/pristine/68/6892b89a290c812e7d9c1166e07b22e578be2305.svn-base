#include "breakpoint_dialog.h"
#include "ui_breakpoint_dialog.h"
#include <QRadioButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QCheckBox>


breakpoint_dialog::breakpoint_dialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::breakpoint_dialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);

}

breakpoint_dialog::~breakpoint_dialog()
{
    delete ui;
}

void breakpoint_dialog::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}



BREAKPOINT breakpoint_dialog::SetBreakPoint()
{
    BREAKPOINT ret;
    memset(&ret, 0, sizeof(BREAKPOINT));

    QRadioButton *button;
    QLineEdit *textfield;
    QCheckBox *chkbox;
    bool _alright;



    button = this->findChild<QRadioButton *>("radioIRQ");

    if(button->isChecked())
      ret.break_at_irq = true;

    button = this->findChild<QRadioButton *>("radioNMI");
    if(button->isChecked())
    ret.break_at_nmi = true;



    button = this->findChild<QRadioButton *>("radioBRK");
    if(button->isChecked())
      ret.break_at_brk = true;



    button = this->findChild<QRadioButton *>("radioOpcode");
    if(button->isChecked())
    {
        textfield = this->findChild<QLineEdit *>("opcode_linedit");

         ret.break_on_opcode = textfield->text().toUShort(&_alright, 16);
         if(_alright == false)
         {
             QMessageBox::warning(this, tr("arrrror!"), tr("You can only enter hexadecimal internetz in the opcode field!"), QMessageBox::Ok);

         }
         textfield->clear();


    }



    //okay, now for the rest...

    button = this->findChild<QRadioButton *>("radioExec");
    if(button->isChecked())
    {
        textfield = this->findChild<QLineEdit *>("memory_access_adress");
        ret.adress = textfield->text().toUShort(&_alright, 16);
        if(_alright == false)
        {
            QMessageBox::warning(this, tr("arrrror!"), tr("You can only enter hexadecimal internetz in the adress field!"), QMessageBox::Ok);

        }
        textfield->clear();


    }




    chkbox = this->findChild<QCheckBox *>("BP_Enabled_chkbx");
    if(chkbox->isChecked())
        ret.is_active = true;
    else
        ret.is_active = false;



    return ret;

}

void breakpoint_dialog::on_radioOpcode_clicked()
{
    QLineEdit *f;
    f = this->findChild<QLineEdit *>("opcode_linedit");


    f->setFocus();

}

void breakpoint_dialog::on_OK_Button_clicked()
{

    //bp ær breakpointnummret
    int bp =  emu->AddBreakPoint(SetBreakPoint());



    this->close();

}

