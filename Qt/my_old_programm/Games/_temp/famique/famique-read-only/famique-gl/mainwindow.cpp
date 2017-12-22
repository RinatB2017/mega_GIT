#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->m_GLDrawArea = new GLWidget(this);

    this->setCentralWidget(m_GLDrawArea);

    this->m_NES = new NES(m_GLDrawArea);
    //m_GLDrawArea->initializeGL();
//   m_GLDrawArea->paintGL();



}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->m_NES;
}


void MainWindow::paintEvent(QPaintEvent *eve)
{

}

void MainWindow::resizeEvent(QResizeEvent *res)
{
    m_GLDrawArea->resizeGL(width(), height());

}

void MainWindow::on_action_Load_binary_triggered()
{

    QString path;

        path = QFileDialog::getOpenFileName(this,tr("Open ROM file"), this->m_NES->m_ROMpath, tr("ROM files (*.nes *.bin)") );

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

    if(m_NES->isInitialized())
        file_name->clear();


    this->CheckAndStopThreads(); //see if emulator is running, if it is, kill the emulator thread before attempting to re-initialize it with a new program

    if(!m_NES->OpenROM(path.toLatin1().data()))
    {


        file_name->clear();
        file_name->setText(QString("Loaded ")+ get_filename[get_filename.count()-1]);


        if(m_NES->m_ROMpath == 0)
          delete m_NES->m_ROMpath;

            m_NES->m_ROMpath = new char [strlen(path.toLatin1().data())+1];

            memcpy(m_NES->m_ROMpath, path.toLatin1().data(), strlen(path.toLatin1().data()));

    }

    if(this->m_NES->isInitialized())
        Run();



}


void MainWindow::Run()
{
    m_RefreshTimer = new QTimer(this);


    connect(m_RefreshTimer, SIGNAL(timeout()), this, SLOT(UpdateLogic()));
    m_RefreshTimer->start(1);
    m_NES->start();



}

void MainWindow::UpdateLogic()
{


    if(m_NES->m_Draw)
    {
    this->m_GLDrawArea->UpdateTextureData(m_NES->GetCPUHandle()->ppu_2->UpdateTexMem());

    m_GLDrawArea->paintGL();
    this->m_NES->m_Draw = false;
    }


}

void MainWindow::CheckAndStopThreads()
{
    if(m_NES->isInitialized())
    {
        if(m_NES->isRunning())
        {
        m_NES->Stop();
        m_NES->quit();
        }

       if(this->m_RefreshTimer->isActive())
        this->m_RefreshTimer->stop();

   }
}

void MainWindow::keyPressEvent(QKeyEvent *qtKeyEvent)
{

    m_NES->setControllerBits(m_NES->GetInputHandle()->UpdateController1(qtKeyEvent->key(), m_NES->getControllerBits(), true));

}

void MainWindow::keyReleaseEvent(QKeyEvent *qtKeyEvent)
{
    m_NES->setControllerBits(m_NES->GetInputHandle()->UpdateController1(qtKeyEvent->key(), m_NES->getControllerBits(), false));

}
