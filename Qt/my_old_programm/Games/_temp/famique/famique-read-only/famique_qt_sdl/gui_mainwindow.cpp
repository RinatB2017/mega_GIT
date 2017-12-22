


#include "gui_mainwindow.h"
#include "ui_gui_mainwindow.h"
#include "debugdialog.h"
#include "ppu_viewer_dialog.h"
#include <QFileDialog>
#include <QString>
#include <QStringList>
#include <QStatusBar>
#include <QLabel>
#include <QKeyEvent>
#include "sdlwidget.h"


int last_key_pressed;
GUI_MainWindow::GUI_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GUI_MainWindow)
{
    ui->setupUi(this);


    this->file_name = NULL;
    debug_dialog = new DebugDialog();
    this->ppu_dialog = new PPU_viewer_dialog();

    this->FPSTimer = new QTimer(this);
    this->connect(FPSTimer, SIGNAL(timeout()), this, SLOT(updateWindowTitle()));
    FPSTimer->start(1000);//update once a second

    renderer = new  QSDLScreenWidget(this);
    this->setCentralWidget(renderer);

    settings_dialog = new Settings(this);

    frame_limiter = 0;




}

GUI_MainWindow::~GUI_MainWindow()
{

    FPSTimer->stop();
      delete FPSTimer;
    if(frame_limiter)
    {

        frame_limiter->stop();

        delete frame_limiter;
    }
    delete debug_dialog;



    delete emu;

    delete ppu_dialog;

    delete renderer;

    delete ui;


}

void GUI_MainWindow::SetEmulatorPtr(Emulator *ptr)
{
    emu = ptr;
    debug_dialog->SetEmulatorPtr(this->emu);
    ppu_dialog->SetEmuPtr(emu);
    emu->SetSDLSurface(renderer->getSDLSurface());
    renderer->SetEmulatorPtr(emu);
    settings_dialog->SetEmuPtr(emu);
}


void GUI_MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;



    default:
        break;
    }
}

void GUI_MainWindow::paintEvent(QPaintEvent *e)
{

    this->setFocus();

}


void GUI_MainWindow::keyPressEvent(QKeyEvent *e)
{
/*
    byte controller = emu->get_controller();

    switch(e->key())
    {
    case 0x01000012:
            SET_BIT(controller, 6);

    break;
        case 0x01000014:

        SET_BIT(controller, 7);

    break;
    case 0x01000004:

        SET_BIT(controller, 3);



    break;


        case 0x01000013:

                SET_BIT(controller, 4);


    break;

        case 0x01000015:

                SET_BIT(controller, 5);

    break;

        case Qt::Key_A:

                SET_BIT(controller, 0);

    break;

    case Qt::Key_F5:

            if(emu->isInitialized())
            {
                if(emu->isRunning())
                   emu->Stop();
                else
                    emu->Start();

            }

    break;
    case Qt::Key_Z:

                SET_BIT(controller, 1);

    break;
    case Qt::Key_Space:
                SET_BIT(controller, 2);

    break;


                //ppu debug stuff

    case 0x31:
              emu->GetCPUHandle()->ppu.DEBUG_X++;
    break;
    case 0x32:
              emu->GetCPUHandle()->ppu.DEBUG_X--;
    break;
    case 0x33:
              emu->GetCPUHandle()->ppu.DEBUG_Y++;
    break;

    case 0x34:
            emu->GetCPUHandle()->ppu.DEBUG_Y--;
    break;

    case 0x46:
        emu->fast_forward = true;
        break;

    default:
        break;
    }

    emu->set_controller(controller);


*/

    emu->set_controller(emu->GetInputHandle()->UpdateController1(e->key(), emu->get_controller(), true));

}

void GUI_MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    emu->set_controller(emu->GetInputHandle()->UpdateController1(e->key(), emu->get_controller(), false));

 /*   byte controller = emu->get_controller();

    switch(e->key())
    {
    case 0x01000012:
            CLR_BIT(controller, 6);

    break;
        case 0x01000014:

        CLR_BIT(controller, 7);

    break;
    case 0x01000004:

        CLR_BIT(controller, 3);



    break;


        case 0x01000013:

                CLR_BIT(controller, 4);


        break;

        case 0x01000015:

                CLR_BIT(controller, 5);

        break;

        case Qt::Key_A:

                CLR_BIT(controller, 0);

                break;
        case Qt::Key_Z:

                CLR_BIT(controller, 1);

break;

        case Qt::Key_Space:

        CLR_BIT(controller, 2);
        break;

    case 0x46:

        emu->fast_forward = false;
        break;

    default:
        break;
    }

    emu->set_controller(controller);

*/

}

void GUI_MainWindow::on_action_Load_ROM_triggered()
{

    QString path;

        path = QFileDialog::getOpenFileName(this,tr("Open ROM file"), emu->rompath, tr("ROM files (*.nes *.bin)") );

    if(path.isEmpty())
        return;

//    QStatusBar *status = this->findChild<QStatusBar *>("statusBar");
    QStringList get_filename = path.split("/");

    if(this->file_name == NULL)
    {
       this->file_name = new QLabel(this);
       QStatusBar *status = statusBar();


       status->addWidget(file_name, 200);

   }

    if(emu->isInitialized())
        file_name->clear();


    if(!emu->OpenROM(path))
    {


        file_name->clear();
        file_name->setText(QString("Loaded ")+ get_filename[get_filename.count()-1]);


        if(emu->rompath == 0)
          delete emu->rompath;

            emu->rompath = new char [strlen(path.toLatin1().data())+1];

            memcpy(emu->rompath, path.toLatin1().data(), strlen(path.toLatin1().data()));
      if(emu->GetDASMHandle() != NULL)
          debug_dialog->PopulateDisassembly();

    }


}

void GUI_MainWindow::ShowDebugDialog()
{

    if(emu->isRunning())
        emu->Stop();

    debug_dialog->show();
    debug_dialog->UpdateDebugStats();




}

void GUI_MainWindow::on_actionCPU_triggered()
{
    ShowDebugDialog();
}

void GUI_MainWindow::on_action_Exit_triggered()
{


    QApplication::exit();
}

void GUI_MainWindow::closeEvent(QCloseEvent *e)
{


    QApplication::exit();

}

void GUI_MainWindow::on_action_Start_CPU_triggered()
{
    emu->Start();
}

void GUI_MainWindow::on_actionReset_wrappers_triggered()
{

    emu->ZAP();


}

void GUI_MainWindow::on_actionPPU_triggered()
{
    this->ppu_dialog->show();
}

void GUI_MainWindow::updateWindowTitle()
{

    this->setWindowTitle("[NTSC PPU] "+ QString::number((float)(emu->frames_drawn )) + " FPS");

    emu->frames_drawn = 0;


}


void GUI_MainWindow::Start()
{

    this->frame_limiter = new QTimer(renderer);
    renderer->connect(frame_limiter, SIGNAL(timeout()), renderer, SLOT(onReDraw()));
    frame_limiter->start(0);
}

void GUI_MainWindow::on_actionTurn_on_triggered()
{

    emu->frame_limit = !emu->frame_limit;


}

void GUI_MainWindow::on_action_Halt_CPU_triggered()
{
    emu->Stop();
}

void GUI_MainWindow::on_actionEnableSound_triggered()
{
    emu->audio_enabled = !emu->audio_enabled;
    emu->GetCPUHandle()->memory->disable_apu = true;
}

void GUI_MainWindow::on_actionSoft_reset_triggered()
{
    if(emu->isInitialized())
        emu->GetCPUHandle()->Reset();
}

void GUI_MainWindow::on_actionConfigure_triggered()
{
    settings_dialog->show();
    settings_dialog->ShowCurrentKeymap();

}

void GUI_MainWindow::on_actionGo_fullscreen_triggered()
{
    this->renderer->GoFullScreen();
}

void GUI_MainWindow::on_actionDump_stac_triggered()
{
    for(int i=0xba;i<0xff;i++)
        printf("[%02X]: %02X ", i, emu->GetCPUHandle()->memory->ReadMemory(0x100 + i));
}
