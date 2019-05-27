#include "ppu_viewer_dialog.h"
#include "ui_ppu_viewer_dialog.h"

#include <QPainter>
#include <QTimer>

#include "emulator.h"

PPU_viewer_dialog::PPU_viewer_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PPU_viewer_dialog)
{
    ui->setupUi(this);

    //set up some QImages to draw on to
    this->nametable = new QImage(256, 240, QImage::Format_RGB32);


    timer = new QTimer(this);
//    sdlw->connect(new_timer, SIGNAL(timeout()), sdlw, SLOT(onReDraw()));

    this->connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));
nametable_selected = 0;


}

PPU_viewer_dialog::~PPU_viewer_dialog()
{
    delete ui;
    delete nametable;
    if(timer)
    {
        timer->stop();

        delete timer;
    }
}

void PPU_viewer_dialog::changeEvent(QEvent *e)
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

//overloaded function from QWidget
void PPU_viewer_dialog::paintEvent(QPaintEvent *e)
{

    QPainter draw_shit(this);



    if(emu->isInitialized())
        DrawNametables(nametable_selected);

    draw_shit.translate(2,12);
    draw_shit.drawImage(0,0, *nametable);

    draw_shit.resetMatrix();






}

void PPU_viewer_dialog::showEvent(QShowEvent *event)
{
    //start the redraw timer
      timer->start(50);





}

void PPU_viewer_dialog::closeEvent(QCloseEvent *event)
{

    timer->stop();

}

void PPU_viewer_dialog::DrawNametables(int nt)
{
    byte *bytes = (byte *)nametable->bits();
    memset(bytes, 0x00, 240*256*4);
/*
    for(int i=1;i<5;i++)
    {
       if(i==1)
    bytes =  (unsigned long *)nametable_1->bits();
        if(i==2)
    bytes =  (unsigned long *)nametable_2->bits();

        if(i==3)
    bytes = (unsigned long *)nametable_3->bits();

        if(i==4)
    bytes =  (unsigned long *)nametable_4->bits();
*/


    byte *namet = (byte *)emu->GetNameTableDebug(nt);

  /*  for(int i=0;i<0xf000;i++)
    {
        bytes[i] = namet[i];
     //   bytes[i] = ((0xFF ) << 24) + nt[i];
    }
*/
    memcpy(bytes, namet, 0xf000 * 4);


    delete [] namet;







}

void PPU_viewer_dialog::on_radio2000_clicked()
{
  nametable_selected = 1;
    repaint();
}

void PPU_viewer_dialog::on_radio2400_clicked()
{
  nametable_selected = 2;

    repaint();
}


void PPU_viewer_dialog::on_radio2800_clicked()
{
  nametable_selected = 3;
    repaint();
}

void PPU_viewer_dialog::on_radio2C00_clicked()
{
   nametable_selected = 4;
    repaint();
}
