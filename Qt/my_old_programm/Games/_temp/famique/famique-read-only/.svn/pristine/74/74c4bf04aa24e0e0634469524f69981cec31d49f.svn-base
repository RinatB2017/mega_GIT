#include "debugdialog.h"
#include "ui_debugdialog.h"
#include <QString>
#include <QStringList>
#include <QPlainTextEdit>
#include <QTextCursor>
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QModelIndex>
#include <QTableWidget>


DebugDialog::DebugDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebugDialog)
{
    ui->setupUi(this);
    this->bp_dlg = new breakpoint_dialog(parent);

    disable_redraw = false;



}

DebugDialog::~DebugDialog()
{
    delete ui;
    delete bp_dlg;

}

void DebugDialog::changeEvent(QEvent *e)
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


void DebugDialog::PopulateDisassembly()
{

    QString dasm = QString(QLatin1String(this->emu->GetDASMHandle()->GetDisassembly()));
    QStringList splitup_disassembly = dasm.split("\n");




    QPlainTextEdit *disassembly_view = DebugDialog::findChild<QPlainTextEdit*>("disassembly_view");
    disassembly_view->clear();



//    for(int i=0; i<splitup_disassembly.count(); i++)
  //  {
        disassembly_view->appendPlainText(dasm);
  //  }

    disassembly_view->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);


    byte *mem = emu->GetCPUHandle()->memory->GetMemoryHandle();
    QLabel *update_lbls = this->findChild<QLabel *>("NMI_label");
    update_lbls->setText("NMI: $" + QString::number(MAKE_WORD(emu->GetCPUHandle()->memory->ReadMemory(0xFFFA), emu->GetCPUHandle()->memory->ReadMemory(0xFFFB)),16));
    update_lbls = this->findChild<QLabel *>("BRK_label");
    update_lbls->setText("BRK/IRQ: $" + QString::number(MAKE_WORD(emu->GetCPUHandle()->memory->ReadMemory(0xFFFE), emu->GetCPUHandle()->memory->ReadMemory(0xFFFF)),16));


}

void DebugDialog::paintEvent(QPaintEvent *)
{

    if(emu->isInitialized())
    {
        if(emu->isRunning())
            debug_snapshot = emu->GetCPUHandle()->GetCPUState();

        UpdateDebugStats();
        if(emu->BP_occured())
        {
            disable_redraw = false;
            highlightbreakpoint();
            UpdateDebugStats();
            disable_redraw = true;
        }
    }
}



void DebugDialog::on_STEPbutton_clicked()
{
    if(emu->isRunning())
        emu->Stop(); //halt normal execution

    if(emu->isInitialized())
       this->debug_snapshot = emu->StepCPU();


    this->disable_redraw = false;


    UpdateDebugStats();
    highlightbreakpoint();
    this->disable_redraw = true;



}

void DebugDialog::on_pushButton_clicked()
{
    QLineEdit *PC_str;
    QPlainTextEdit *dasm_field;

    QString find_str;
    PC_str = this->findChild<QLineEdit *>("PC_field");


    find_str = PC_str->text() + ":";

    dasm_field = this->findChild<QPlainTextEdit *>("disassembly_view");
    //dasm_field->setTextCursor(QTextCursor::Start);
    dasm_field->find(find_str, QTextDocument::FindBackward);
    dasm_field->find(find_str);
    dasm_field->setFocus();


}

void DebugDialog::on_add_breakpoint_button_clicked()
{

        bp_dlg->show();
}

void DebugDialog::on_pushButton_2_clicked()
{
    emu->Start();
    this->disable_redraw = true;
}

void DebugDialog::on_pushButton_4_clicked()
{
    emu->Stop();
    this->disable_redraw = false;
}

void DebugDialog::on_PC_field_returnPressed()
{
    on_pushButton_clicked();
}

void DebugDialog::on_PC_field_selectionChanged()
{
    //QMessageBox::warning(this, "wee","wee", "fsck you!");
}

void DebugDialog::highlightbreakpoint()
{

//    CPU_SNAPSHOT snap = emu->GetCPUHandle()->GetCPUState();
    QLineEdit *PC_str = this->findChild<QLineEdit *>("PC_field");
    PC_str->clear();
    PC_str->setText(QString::number(debug_snapshot.pc, 16));

    on_pushButton_clicked();




    
}



void DebugDialog::on_remove_breakpointButton_clicked()
{
    QListWidget *bps = this->findChild<QListWidget *>("bp_table_widget");

    for(int i=0; i<bps->count(); i++)
    {
        if(bps->item(i)->isSelected())
        {

            //this is the breakpoint we'd like to remove... so remove it :)
            emu->RemoveBreakPoint(i);
        }
    }
    disable_redraw = false;
}

void DebugDialog::on_bp_table_widget_itemClicked(QListWidgetItem* item)
{
    this->disable_redraw = true;


}

void DebugDialog::on_resetButton_clicked()
{
    //emu->GetCPUHandle()->Init();
    emu->GetCPUHandle()->Reset();
}





void DebugDialog::UpdateDebugStats()
{
    QLineEdit *upd;
    QCheckBox *chk;
    QLabel *lbl;
    QListWidget *bp_list;

    //blah, make this look more nice sometime..
    if(!emu->isInitialized() || disable_redraw )
        return;



    //update all text fields accordingly.

    upd = this->findChild<QLineEdit *>("A_field");
    upd->clear();
    upd->setText(QString::number(debug_snapshot.a, 16));

    upd = this->findChild<QLineEdit *>("X_field");
    upd->clear();
    upd->setText(QString::number(debug_snapshot.x, 16));

    upd = this->findChild<QLineEdit *>("Y_field");
    upd->clear();
    upd->setText(QString::number(debug_snapshot.y, 16));

    upd = this->findChild<QLineEdit *>("SP_field");
    upd->clear();
    upd->setText(QString::number(debug_snapshot.SP, 16));

    upd = this->findChild<QLineEdit *>("SR_field");
    upd->clear();
    upd->setText(QString::number(debug_snapshot.fl.SR, 16));


    chk = this->findChild<QCheckBox *>("chk_N");

    chk->setChecked( debug_snapshot.fl.members.N == 1 ? true : false   );
    //chk->setCheckable(false);


    chk = this->findChild<QCheckBox *>("chk_V");
    chk->setChecked( debug_snapshot.fl.members.V == 1  ? true : false   );
    //chk->setCheckable(false);


    chk = this->findChild<QCheckBox *>("chk_U");
    chk->setChecked( debug_snapshot.fl.members.unused == 1 ? true : false   );
  //  chk->setCheckable(false);


    chk = this->findChild<QCheckBox *>("chk_C");
    chk->setChecked( debug_snapshot.fl.members.C ==1 ? true : false   );
 //   chk->setCheckable(false);

    chk = this->findChild<QCheckBox *>("chk_D");
    chk->setChecked( debug_snapshot.fl.members.D ==1 ? true : false   );
   // chk->setCheckable(false);


    chk = this->findChild<QCheckBox *>("chk_I");
    chk->setChecked( debug_snapshot.fl.members.I == 1? true : false   );
    //chk->setCheckable(false);


    chk = this->findChild<QCheckBox *>("chk_B");
    chk->setChecked( debug_snapshot.fl.members.B == 1 ? true : false   );
   // chk->setCheckable(false);


    chk = this->findChild<QCheckBox *>("chk_Z");
    chk->setChecked( debug_snapshot.fl.members.Z == 1 ? true : false   );
   // chk->setCheckable(false);

    lbl = this->findChild<QLabel *>("scanline_lbl");
    lbl->clear();
    lbl->setText(QString("Scanline ")+QString::number(emu->GetCPUHandle()->ppu.num_scanline, 10));




    upd = this->findChild<QLineEdit *>("PC_field");
    upd->clear();
    upd->setText(QString::number(debug_snapshot.pc, 16));


    bp_list = this->findChild<QListWidget *>("bp_table_widget");
    bp_list->clear();
    for(int i=0; i<emu->breakpoints.size();i++)
    {
        BREAKPOINT b = emu->breakpoints[i];

        if(b.break_at_brk)
          bp_list->addItem(QString("break on BRK"));
        else if(b.break_at_irq)
          bp_list->addItem(QString("break on IRQ"));
        else if(b.break_at_nmi)
          bp_list->addItem(QString("break on NMI"));
        else if(b.break_on_opcode != 0)
            bp_list->addItem(QString("break on opcode: ") + QString::number(b.break_on_opcode, 16));
        else if(b.adress)
            bp_list->addItem(QString("break on exec $") + QString::number(b.adress, 16));


    }






    QPlainTextEdit *mem_view = this->findChild<QPlainTextEdit *>("mem_view");

    QString mem_o;
 //   mem_view->setDisabled(true);
    mem_view->clear();
    for(int row=0;row<128;row++)
    {
        mem_o.clear();
        mem_o = QString().number(row * 16, 16) + QString(": ");
        for(int x=0;x<16;x++)
        {
            mem_o = mem_o +  QString().number(emu->GetCPUHandle()->memory->ReadMemory((row *16)+ x), 16) + ((x==15)? QString("\n"):QString(" "));
        }

        mem_view->appendPlainText(mem_o);



    }
 //   mem_view->setEnabled(true);




//    on_pushButton_clicked();



}

