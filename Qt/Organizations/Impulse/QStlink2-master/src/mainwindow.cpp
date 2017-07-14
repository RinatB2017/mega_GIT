/*
    This file is part of QSTLink2.

    QSTLink2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    QSTLink2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with QSTLink2.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <mainwindow.h>
#include <ui_mainwindow.h>
#include <stdlib.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->b_disconnect->setEnabled(false);
    this->ui->gb_top->setEnabled(false);
    this->ui->b_send->setEnabled(false);
    this->ui->b_receive->setEnabled(false);
    this->ui->b_verify->setEnabled(false);
    this->ui->b_repeat->setEnabled(false);
    this->stlink = new stlinkv2();
    this->devices = new DeviceList(this);
    this->tfThread = new transferThread();

    this->lastAction = ACTION_NONE;

    if (this->devices->IsLoaded()) {

        this->ui->gb_top->setEnabled(true);
        this->log(QString::number(this->devices->getDevicesCount())+" Device descriptions loaded.");
        QObject::connect(this->ui->b_quit,SIGNAL(clicked()),this,SLOT(Quit()));
        QObject::connect(this->ui->b_qt,SIGNAL(clicked()),qApp,SLOT(aboutQt()));
        QObject::connect(this->ui->b_connect, SIGNAL(clicked()), this, SLOT(Connect()));
        QObject::connect(this->ui->b_disconnect, SIGNAL(clicked()), this, SLOT(Disconnect()));
        QObject::connect(this->ui->b_send, SIGNAL(clicked()), this, SLOT(Send()));
        QObject::connect(this->ui->b_receive, SIGNAL(clicked()), this, SLOT(Receive()));
        QObject::connect(this->ui->b_verify, SIGNAL(clicked()), this, SLOT(Verify()));
        QObject::connect(this->ui->b_repeat, SIGNAL(clicked()), this, SLOT(Repeat()));
        QObject::connect(this->ui->b_halt, SIGNAL(clicked()), this, SLOT(HaltMCU()));
        QObject::connect(this->ui->b_run, SIGNAL(clicked()), this, SLOT(RunMCU()));
        QObject::connect(this->ui->b_reset, SIGNAL(clicked()), this, SLOT(ResetMCU()));
        QObject::connect(this->ui->r_jtag, SIGNAL(clicked()), this, SLOT(setModeJTAG()));
        QObject::connect(this->ui->r_swd, SIGNAL(clicked()), this, SLOT(setModeSWD()));
        QObject::connect(this->ui->b_hardReset, SIGNAL(clicked()), this, SLOT(HardReset()));

        // Thread
        QObject::connect(this->tfThread, SIGNAL(sendProgress(quint32)), this, SLOT(updateProgress(quint32)));
        QObject::connect(this->tfThread, SIGNAL(sendStatus(QString)), this, SLOT(updateStatus(QString)));
        QObject::connect(this->tfThread, SIGNAL(sendLoaderStatus(QString)), this, SLOT(updateLoaderStatus(QString)));
        QObject::connect(this->stlink, SIGNAL(bufferPct(quint32)), this, SLOT(updateLoaderPct(quint32)));
        QObject::connect(this->tfThread, SIGNAL(sendLock(bool)), this, SLOT(lockUI(bool)));
        QObject::connect(this->ui->b_stop, SIGNAL(clicked()), this->tfThread, SLOT(halt()));
        QObject::connect(this->tfThread, SIGNAL(sendLog(QString)), this, SLOT(log(QString)));

        // Help
        QObject::connect(this->ui->b_help, SIGNAL(clicked()), this, SLOT(showHelp()));
    }

    else {
        this->log("Could not load the devices list");
    }
}

MainWindow::~MainWindow()
{
    this->tfThread->exit();
    delete tfThread;
    delete stlink;
    delete devices;
    delete ui;
}

void MainWindow::showHelp()
{

    this->dialog.setText("Help","Could no load help file");

    QFile file(":/help.html");
    if (!file.open(QIODevice::ReadOnly)) {
        qCritical("Could not open the help file.");
    }

    this->dialog.setHTML(QString("Help"), QString(file.readAll()));
    this->dialog.show();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    (void)event;
    this->Quit();
}

bool MainWindow::Connect()
{
    PrintFuncName();
    this->log("Searching Device...");

    switch (this->stlink->connect()) {
    case -1:
        this->log("ST Link V2 not found or unable to access it.");
#if defined(QWINUSB) && defined(WIN32)
        this->log("Did you install the official ST-Link V2 driver ?");
#elif !defined(WIN32)
        this->log("Did you install the udev rules ?");
#else
        this->log("Did you install the libusb-win32 driver ?");
#endif
        return false;
    default:
        this->log("ST Link V2 found!");
        this->getVersion();
        this->stlink->setExitModeDFU();
        if (this->ui->r_jtag->isChecked())
            this->setModeJTAG();
        else
            this->setModeSWD();
        this->getStatus();
        this->ui->b_connect->setEnabled(false);
        this->ui->b_disconnect->setEnabled(true);
        if (this->getMCU()) {
            this->ui->gb_bottom->setEnabled(true);
            this->ui->b_send->setEnabled(true);
            this->ui->b_receive->setEnabled(true);
            this->ui->b_verify->setEnabled(true);
            this->ui->b_repeat->setEnabled(true);
            return true;
        }
        else
            return false;
    }
    return false;
}

void MainWindow::Disconnect()
{
    this->log("Disconnecting...");
    this->stlink->disconnect();
    this->log("Disconnected.");
    qInformal() << "Disconnected.";
    this->ui->b_disconnect->setEnabled(false);
    this->ui->b_connect->setEnabled(true);
    this->ui->gb_bottom->setEnabled(false);
    this->ui->b_send->setEnabled(false);
    this->ui->b_receive->setEnabled(false);
    this->ui->b_verify->setEnabled(false);
    this->ui->b_repeat->setEnabled(false);
}

void MainWindow::log(const QString &s)
{
    this->ui->t_log->appendPlainText(s);
}

void MainWindow::lockUI(bool enabled)
{
    this->ui->gb_top->setEnabled(!enabled);
    this->ui->gb_bottom->setEnabled(!enabled);
}

void MainWindow::updateProgress(quint32 p)
{
    this->ui->pgb_transfer->setValue(p);
}

void MainWindow::updateStatus(const QString &s)
{
    this->ui->l_progress->setText(s);
}

void MainWindow::updateLoaderStatus(const QString &s)
{
    this->ui->l_status->setText(s);
}

void MainWindow::updateLoaderPct(quint32 p)
{
    this->ui->pgb_loader->setValue(p);
}

void MainWindow::Send()
{
    this->filename.clear();
    this->filename = QFileDialog::getOpenFileName(this, "Open file", "", "Binary Files (*.bin)");
    if (!this->filename.isNull()) {
        QFile file(this->filename);
        if (!file.open(QIODevice::ReadOnly)) {
            qCritical("Could not open the file.");
            return;
        }
        this->log("Size: "+QString::number(file.size()/1024)+"KB");

        if (file.size() > (*this->stlink->device)["flash_size"]*1024) {
            if(QMessageBox::question(this, "Flash size exceeded", "The file is bigger than the flash size!\n\nThe flash memory will be erased and the new file programmed, continue?", QMessageBox::Yes|QMessageBox::No) != QMessageBox::Yes){
                return;
            }
        }
        else {
            if(QMessageBox::question(this, "Confirm", "The flash memory will be erased and the new file programmed, continue?", QMessageBox::Yes|QMessageBox::No) != QMessageBox::Yes){
                return;
            }
        }
        file.close();

        this->Send(this->filename);
        this->lastAction = ACTION_SEND;
    }
}

void MainWindow::Send(const QString &path)
{
    qDebug("Writing flash");
    this->log("Sending "+path);
    this->stlink->resetMCU(); // We stop the MCU
    this->ui->tabw_info->setCurrentIndex(3);
    this->ui->pgb_transfer->setValue(0);
    this->ui->l_progress->setText("Starting transfer...");

    // Transfer thread
    this->tfThread->setParams(this->stlink, path, true, false);
    this->tfThread->start();
}

void MainWindow::Receive()
{
    qDebug("Reading flash");
    this->filename.clear();
    this->filename = QFileDialog::getSaveFileName(this, "Save File", "", "Binary Files (*.bin)");
    if (!this->filename.isNull()) {
        QFile file(this->filename);
        if (!file.open(QIODevice::ReadWrite)) {
            qCritical("Could not save the file.");
            return;
        }
        file.close();
        this->Receive(this->filename);
        this->lastAction = ACTION_RECEIVE;
    }
}

void MainWindow::Receive(const QString &path)
{
    this->log("Saving to "+path);
    this->ui->tabw_info->setCurrentIndex(3);
    this->ui->pgb_transfer->setValue(0);
    this->ui->l_progress->setText("Starting transfer...");

    // Transfer thread
    this->tfThread->setParams(this->stlink, path, false, false);
    this->tfThread->start();
}

void MainWindow::Verify()
{
    qDebug("Verify flash");
    this->filename.clear();
    this->filename = QFileDialog::getOpenFileName(this, "Open file", "", "Binary Files (*.bin)");
    if (!this->filename.isNull()) {
        QFile file(this->filename);
        if (!file.open(QIODevice::ReadOnly)) {
            qCritical("Could not open the file.");
            return;
        }
        file.close();
        this->Verify(this->filename);
        this->lastAction = ACTION_VERIFY;
    }
}

void MainWindow::Repeat()
{
    switch (this->lastAction) {

        case ACTION_SEND:
            this->Send(this->filename);
            break;
        case ACTION_RECEIVE:
            this->Receive(this->filename);
            break;
        case ACTION_VERIFY:
            this->Verify(this->filename);
            break;
        case ACTION_NONE:
            this->log("Nothing to repeat.");
            break;
        default:
            break;
    }
}

void MainWindow::Verify(const QString &path)
{
    this->log("Verifying "+path);
    this->ui->tabw_info->setCurrentIndex(3);
    this->ui->pgb_transfer->setValue(0);
    this->ui->l_progress->setText("Starting Verification...");

    // Transfer thread
    this->tfThread->setParams(this->stlink, path, false, true);
    this->tfThread->start();
}

void MainWindow::eraseFlash()
{
    this->stlink->hardResetMCU();
    this->stlink->resetMCU();
    if (!this->stlink->unlockFlash())
        return;
    this->stlink->eraseFlash();
}

void MainWindow::HaltMCU()
{
    this->log("Halting MCU...");
    this->stlink->haltMCU();
    usleep(100000);
    this->getStatus();
}

void MainWindow::RunMCU()
{
    this->log("Resuming MCU...");
    this->stlink->runMCU();
    usleep(100000);
    this->getStatus();
}

void MainWindow::ResetMCU()
{
    this->log("Reseting MCU...");
    this->stlink->resetMCU();
    usleep(100000);
    this->getStatus();
}

void MainWindow::HardReset()
{
    this->log("Hard Reset...");
    this->stlink->hardResetMCU();
    usleep(100000);
    this->getStatus();
}

void MainWindow::setModeJTAG()
{
    if (!this->stlink->isConnected())
        return;
    this->log("Changing mode to JTAG...");
    this->stlink->setModeJTAG();
    usleep(100000);
    this->getMode();
}

void MainWindow::setModeSWD()
{
    if (!this->stlink->isConnected())
        return;
    this->log("Changing mode to SWD...");
    this->stlink->setModeSWD();
    usleep(100000);
    this->getMode();
}

void MainWindow::Quit()
{
    this->hide();
    if (this->stlink->isConnected())
        this->Disconnect();
    qApp->quit();
}

void MainWindow::getVersion()
{
    this->log("Fetching version...");
    this->stlink->getVersion();
}

void MainWindow::getMode()
{
    this->log("Fetching mode...");
    const quint8 mode = this->stlink->getMode();
    QString mode_str;
    switch (mode) {
        case STLink::Mode::UNKNOWN:
            mode_str = "Unknown";
            break;
        case STLink::Mode::DFU:
            mode_str = "DFU";
            break;
        case STLink::Mode::MASS:
            mode_str = "Mass Storage";
            break;
        case STLink::Mode::DEBUG:
            mode_str = "Debug";
            break;
        default:
            mode_str = "Unknown";
            break;
        }
        this->log("Mode: "+mode_str);
}

void MainWindow::getStatus()
{
    this->log("Fetching status...");
    const quint8 status = this->stlink->getStatus();
    QString status_str;
    switch (status) {
        case STLink::Status::CORE_RUNNING:
            status_str = "Core Running";
            break;
        case STLink::Status::CORE_HALTED:
            status_str = "Core Halted";
            break;
        default:
            status_str = "Unknown";
            break;
        }
        this->log("Status: "+status_str);
}

bool MainWindow::getMCU()
{
    this->log("Fetching MCU Info...");
    this->stlink->getCoreID();
    this->stlink->resetMCU();
    this->stlink->getChipID();

    if (this->devices->search(this->stlink->chip_id)) {
        this->stlink->device = this->devices->cur_device;
        qInformal() << "Device type: " << this->stlink->device->type;

        this->ui->le_type->setText(this->stlink->device->type);
        this->ui->le_chipid->setText("0x"+QString::number((*this->stlink->device)["chip_id"], 16));
        this->ui->le_flashbase->setText("0x"+QString::number((*this->stlink->device)["flash_base"], 16));
        //this->ui->le_flashsize->setText(QString::number((*this->stlink->device)["flash_size"]/1024)+"KB");
        this->ui->le_ramsize->setText(QString::number((*this->stlink->device)["sram_size"]/1024)+"KB");
        this->ui->le_rambase->setText("0x"+QString::number((*this->stlink->device)["sram_base"], 16));

        this->ui->le_stlver->setText(QString::number(this->stlink->version.stlink));
        this->ui->le_jtagver->setText(QString::number(this->stlink->version.jtag));
        this->ui->le_swimver->setText(QString::number(this->stlink->version.swim));

        if(!this->stlink->version.stlink)
            this->ui->le_jtagver->setToolTip("Not supported");
        if(!this->stlink->version.swim)
            this->ui->le_swimver->setToolTip("Not supported");

        (*this->stlink->device)["flash_size"] = this->stlink->readFlashSize();
        this->ui->le_flashsize->setText(QString::number((*this->stlink->device)["flash_size"])+"KB");

        return true;
    }
    this->log("Device not found in database!");
    qCritical() << "Device not found in database!";
    return false;
}
