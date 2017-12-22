/*
 * This file is part of Packet Sender
 *
 * Licensed GPL v2
 * http://PacketSender.com/
 *
 * Copyright Dan Nagle
 *
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QInputDialog>
#include <QPainter>
#include <QDesktopServices>
#include <QScrollBar>
#include <QFileDialog>
#include <QLibrary>
#include <QFile>
#include <QTime>
#include <QHostAddress>
#include <QHostInfo>
#include <QShortcut>
#include <QClipboard>
#include "brucethepoodle.h"
#include "settings.h"
#include "about.h"
#include "subnetcalc.h"


int hexToInt(QChar hex);


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timeSinceLaunch();

    QSettings settings(SETTINGSFILE, QSettings::IniFormat);


    hyperLinkStyle = "QPushButton { color: blue; } QPushButton::hover { color: #BC810C; } ";

    QIcon mIcon(":pslogo.png");


    lastSendPacket.clear();



    setWindowTitle("Packet Sender");
    setWindowIcon(mIcon);

    tableActive = false;


    maxLogSize = 0;

    if(settings.value("rolling500entryCheck", false).toBool()) {
        maxLogSize = 100;
    }

    asciiEditTranslateEBCDIC = settings.value("asciiEditTranslateEBCDICCheck", false).toBool();

    http = new QNetworkAccessManager(this); //the only http object (required by Qt)

    QDEBUG() << " http connect attempt:" << connect(http, SIGNAL(finished(QNetworkReply*)),
         this, SLOT(httpFinished(QNetworkReply*)));
    //http->get(QNetworkRequest(QUrl("http://packetsender.com/")));


    QDEBUG() << " packet send connect attempt:" << connect(this, SIGNAL(sendPacket(Packet)),
         &packetNetwork, SLOT(packetToSend(Packet)));

    packetNetwork.init();

    packetNetwork.responseData = settings.value("responseHex","").toString();

    ui->persistentTCPCheck->setChecked(settings.value("persistentTCPCheck", false).toBool());
    packetNetwork.persistentConnectCheck = ui->persistentTCPCheck->isChecked();


    //load last session
    if(settings.value("restoreSessionCheck", true).toBool()) {
        QDEBUG() << "Restoring last session";
        //ui->packetNameEdit->setText(settings.value("packetNameEditSession","").toString());
        ui->packetIPEdit->setText(settings.value("packetIPEditSession","").toString());
        ui->packetHexEdit->setText(settings.value("packetHexEditSession","").toString());
        QString methodchoice = settings.value("udptcpComboBoxSession","TCP").toString();
        int findtext = ui->udptcpComboBox->findText(methodchoice);
        if(findtext > -1) {
            ui->udptcpComboBox->setCurrentIndex(findtext);
        }
        ui->packetPortEdit->setText(settings.value("packetPortEditSession","").toString());
        ui->resendEdit->setText(settings.value("resendEditSession","").toString());

        if(!ui->packetHexEdit->text().isEmpty()) {
            on_packetHexEdit_lostFocus();
        }

    }




    packetNetwork.sendResponse = settings.value("sendReponse", false).toBool();


    //http->get(QNetworkRequest(QUrl("http://packetsender.com/")));

    //Connect statusbar to packetNetwork
    QDEBUG() << ": packetNetwork -> Statusbar connection attempt" <<
               connect(&packetNetwork, SIGNAL(toStatusBar(const QString &, int, bool)),
                       this, SLOT(statusBarMessage(const QString &, int, bool)));

    //Connect packetNetwork to trafficlog
    QDEBUG() << ": packetSent -> toTrafficLog connection attempt" <<
               connect(&packetNetwork, SIGNAL(packetSent(Packet)),
                       this, SLOT(toTrafficLog(Packet)));


    packetsSaved = Packet::fetchAllfromDB("");
    QDEBUGVAR(packetsSaved.size());

    loadPacketsTable();

 //   statusBar()->insertPermanentWidget(0, generatePSLink());
 //   statusBar()->insertPermanentWidget(1, generateDNLink());



    stopResendingButton = new QPushButton("Resending");
    stopResendingButton->setStyleSheet("QPushButton { color: black; } QPushButton::hover { color: #BC810C; } ");
    stopResendingButton->setFlat(true);
    stopResendingButton->setCursor(Qt::PointingHandCursor);
    stopResendingButton->setIcon(QIcon(PSLOGO));

    connect(stopResendingButton, SIGNAL(clicked()),
            this, SLOT(cancelResends()));


    statusBar()->insertPermanentWidget(1, stopResendingButton);

    stopResendingButton->hide();

    udpServerStatus = new QPushButton("UDP:"+QString::number(packetNetwork.getUDPPort()));
    udpServerStatus->setStyleSheet("QPushButton { color: black; } QPushButton::hover { color: #BC810C; } ");
    udpServerStatus->setFlat(true);
    udpServerStatus->setCursor(Qt::PointingHandCursor);
    udpServerStatus->setIcon(QIcon(UDPRXICON));

    connect(udpServerStatus, SIGNAL(clicked()),
            this, SLOT(toggleUDPServer()));


    statusBar()->insertPermanentWidget(2, udpServerStatus);


    //updatewidget
    tcpServerStatus = new QPushButton("TCP:"+QString::number(packetNetwork.getTCPPort()));
    tcpServerStatus->setStyleSheet("QPushButton { color: black; } QPushButton::hover { color: #BC810C; } ");
    tcpServerStatus->setFlat(true);
    tcpServerStatus->setCursor(Qt::PointingHandCursor);
    tcpServerStatus->setIcon(QIcon(TCPRXICON));


    connect(tcpServerStatus, SIGNAL(clicked()),
            this, SLOT(toggleTCPServer()));


    statusBar()->insertPermanentWidget(3, tcpServerStatus);



    IPv4Stylesheet = "QPushButton {width:50px; color: green; } QPushButton::hover { color: #BC810C; } ";
    IPv6Stylesheet = "QPushButton {width:50px; color: blue; } QPushButton::hover { color: #BC810C; } ";

    //ipmode toggle
    IPmodeButton = new QPushButton("IPv4 Mode");
    IPmodeButton->setFlat(true);
    IPmodeButton->setCursor(Qt::PointingHandCursor);
    statusBar()->insertPermanentWidget(4, IPmodeButton);

    setIPMode();

    connect(IPmodeButton, SIGNAL(clicked()),
            this, SLOT(toggleIPv4_IPv6()));




    UDPServerStatus();
    TCPServerStatus();

    multiSendDelay = settings.value("multiSendDelay", 0).toFloat();
    cancelResendNum = settings.value("cancelResendNum", 0).toInt();
    resendCounter = 0;



    refreshTimer.setInterval(100);
    qDebug() << __FILE__ << "/" <<__LINE__  << ": refreshTimer Connection attempt " <<
                connect ( &refreshTimer , SIGNAL ( timeout() ) , this, SLOT ( refreshTimerTimeout (  ) ) ) ;

    packetsLogged.clear();
    loadTrafficLogTable();

    refreshTimer.start();

    generateConnectionMenu();


    if(packetsSaved.size() > 0) {
        ui->searchLineEdit->setFocus(); //put cursor in search bar
    }



        //Bruce is my pet poodle.
        //Dog easter egg.  CTRL D, O, G.
        //             or  CMD D, O, G.
    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_D,Qt::CTRL + Qt::Key_O, Qt::CTRL + Qt::Key_G ), this);
    QDEBUG() << ": dog easter egg Connection attempt " << connect(shortcut, SIGNAL(activated()), this, SLOT(poodlepic()));

    QShortcut *shortcutEBCDIC = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_E), this);
    QDEBUG() << ": EBC connection attempt" << connect(shortcutEBCDIC, SIGNAL(activated()), this, SLOT(ebcdicTranslate()));


    QShortcut *field1 = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_1), this);
    QShortcut *field2 = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_2), this);
    QShortcut *field3 = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_3), this);
    QShortcut *field4 = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_4), this);
    QShortcut *field5 = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_5), this);
    QShortcut *field6 = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_6), this);
    QShortcut *field7 = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_7), this);
    QShortcut *field8 = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_8), this);


    if(!connect(field1, &QShortcut::activated, this, &MainWindow::shortcutkey1)) {
        QDEBUG() << "field1 connection false";
    }

    if(!connect(field2, &QShortcut::activated, this, &MainWindow::shortcutkey2)) {
        QDEBUG() << "field2 connection false";
    }

    if(!connect(field3, &QShortcut::activated, this, &MainWindow::shortcutkey3)) {
        QDEBUG() << "field3 connection false";
    }

    if(!connect(field4, &QShortcut::activated, this, &MainWindow::shortcutkey4)) {
        QDEBUG() << "field4 connection false";
    }

    if(!connect(field5, &QShortcut::activated, this, &MainWindow::shortcutkey5)) {
        QDEBUG() << "field5 connection false";
    }
    if(!connect(field6, &QShortcut::activated, this, &MainWindow::shortcutkey6)) {
        QDEBUG() << "field6 connection false";
    }

    if(!connect(field7, &QShortcut::activated, this, &MainWindow::shortcutkey7)) {
        QDEBUG() << "field7 connection false";
    }

    if(!connect(field8, &QShortcut::activated, this, &MainWindow::on_testPacketButton_clicked)) {
        QDEBUG() << "field8 connection false";
    }


    //Now that the UI is loaded, create the settings folders if they do not exist
    QDir mdir;
    mdir.mkpath(TEMPPATH);
    mdir.mkpath(SETTINGSPATH);


    QDEBUG() << "Load time:" <<  timeSinceLaunch();

    QDEBUG() << "Settings file loaded" << SETTINGSFILE;
    QDEBUG() << "Packets file loaded" << PACKETSFILE;

}

void MainWindow::ebcdicTranslate()
{
    QDEBUG() << "Translate ascii field";
    on_packetASCIIEdit_lostFocus();

    QDEBUGVAR(asciiEditTranslateEBCDIC);
    if(asciiEditTranslateEBCDIC) {
        Packet ebcdicPkt;
        ebcdicPkt.init();
        ebcdicPkt.hexString = ui->packetHexEdit->text();
        QString oldascii = ui->packetASCIIEdit->text();
        QByteArray ebcdic = Packet::ASCIItoEBCDIC(ebcdicPkt.getByteArray());
        ebcdicPkt.hexString = Packet::byteArrayToHex(ebcdic);
        ui->packetASCIIEdit->setText(ebcdicPkt.asciiString());
        ui->packetHexEdit->setText(ebcdicPkt.hexString);

        ui->packetASCIIEdit->setToolTip("EBCDIC: " + oldascii);
    }
}

void MainWindow::toggleUDPServer()
{
    QSettings settings(SETTINGSFILE, QSettings::IniFormat);
    settings.setValue("udpServerEnable", !settings.value("udpServerEnable", true).toBool());
    applyNetworkSettings();
}
void MainWindow::toggleTCPServer()
{
    QSettings settings(SETTINGSFILE, QSettings::IniFormat);
    settings.setValue("tcpServerEnable", !settings.value("tcpServerEnable", true).toBool());
    applyNetworkSettings();
}

void MainWindow::generateConnectionMenu() {


}


QPushButton * MainWindow::generatePSLink()
{
    QPushButton * returnButton = new QPushButton("PacketSender.com");
    returnButton->setStyleSheet("QPushButton { color: blue; } QPushButton::hover { color: #BC810C; } ");
    returnButton->setIcon( QIcon(":pslogo.png"));
    returnButton->setFlat(true);
    returnButton->setCursor(Qt::PointingHandCursor);
    connect(returnButton, SIGNAL(clicked()),
            this, SLOT(gotoPacketSenderDotCom()));

    return returnButton;

}

void MainWindow::toTrafficLog(Packet logPacket)
{

    QDEBUG() << logPacket.name;

    if(ui->logTrafficCheck->isChecked())
    {
        if(!logPacket.toIP.isEmpty() && !logPacket.fromIP.isEmpty()) {
            packetsLogged.append(logPacket);
        }
    }

}


void MainWindow::UDPServerStatus()
{
    if(packetNetwork.getUDPPort() > 0)
    {
        udpServerStatus->setText("UDP:"+QString::number(packetNetwork.getUDPPort()));

    } else {
        udpServerStatus->setText("UDP Server Disabled");

    }

    //updatewidget


}

void MainWindow::TCPServerStatus()
{
    if(packetNetwork.getTCPPort() > 0)
    {
        tcpServerStatus->setText("TCP:"+QString::number(packetNetwork.getTCPPort()));
    } else {
        tcpServerStatus->setText("TCP Server Disabled");

    }


}

QPushButton *MainWindow::generateDNLink()
{

    QPushButton * returnButton = new QPushButton("DanNagle.com");
    returnButton->setStyleSheet("QPushButton { color: blue; } QPushButton::hover { color: #BC810C; } ");
    returnButton->setIcon( QIcon(":dannagle_logo.png"));
    returnButton->setFlat(true);
    returnButton->setCursor(Qt::PointingHandCursor);
    connect(returnButton, SIGNAL(clicked()),
            this, SLOT(gotoDanNagleDotCom()));

    return returnButton;

}

void MainWindow::loadTrafficLogTable()
{

    QTableWidgetItem * tItem;
    Packet tempPacket;

    QSettings settings(SETTINGSFILE, QSettings::IniFormat);


    packetTableHeaders  = Settings::defaultTrafficTableHeader();
    packetTableHeaders = settings.value("packetTableHeaders", packetTableHeaders).toStringList();
    ui->trafficLogTable->setColumnCount(packetTableHeaders.size());

    ui->trafficLogTable->verticalHeader()->show();
    ui->trafficLogTable->horizontalHeader()->show();
    ui->trafficLogTable->setHorizontalHeaderLabels(packetTableHeaders);


    QList<Packet> displayPackets;
    displayPackets.clear();


    QString filterString = ui->searchLineEdit->text().toLower().trimmed();


    filterString.clear(); // don't search log

    if(filterString.isEmpty())
    {
        displayPackets = packetsLogged;
    } else {
        foreach(tempPacket, packetsLogged)
        {

            if(tempPacket.name.toLower().contains(filterString) ||
                    tempPacket.toIP.contains(filterString) ||
                    tempPacket.hexToASCII(tempPacket.hexString).toLower().contains(filterString) ||
                    tempPacket.hexString.toLower().contains(filterString)
                    )
            {
                displayPackets.append(tempPacket);
                continue;
            }
        }

    }




    Packet::sortByTime(displayPackets);

    if(displayPackets.isEmpty())
    {
        ui->trafficLogTable->setRowCount(0);
    } else {
        ui->trafficLogTable->setRowCount(displayPackets.count());
    }
    unsigned int rowCounter = 0;

    QDEBUGVAR(packetTableHeaders);

    ui->trafficLogTable->setSortingEnabled(false);

    foreach(tempPacket, displayPackets)
    {

/*

lwStringList ("Time", "From IP", "From Port", "To IP", "To Port", "Method", "Error", "ASCII", "Hex")

*/

        //
        tItem = new QTableWidgetItem(tempPacket.timestamp.toString(DATETIMEFORMAT));
        tItem->setIcon(tempPacket.getIcon());
        Packet::populateTableWidgetItem(tItem, tempPacket);

        ui->trafficLogTable->setItem(rowCounter, packetTableHeaders.indexOf("Time"), tItem);

        tItem = new QTableWidgetItem(tempPacket.fromIP);
        Packet::populateTableWidgetItem(tItem, tempPacket);
        ui->trafficLogTable->setItem(rowCounter, packetTableHeaders.indexOf("From IP"), tItem);

        tItem = new QTableWidgetItem(QString::number(tempPacket.fromPort));
        Packet::populateTableWidgetItem(tItem, tempPacket);
        ui->trafficLogTable->setItem(rowCounter, packetTableHeaders.indexOf("From Port"), tItem);

        tItem = new QTableWidgetItem(tempPacket.toIP);
        Packet::populateTableWidgetItem(tItem, tempPacket);
        ui->trafficLogTable->setItem(rowCounter, packetTableHeaders.indexOf("To IP"), tItem);
        /*
            packetTableHeaders <<"Time" << "From IP" <<"From Port" << "To IP" <<
        "To Port" <<
        "Method"<<"Error" << "ASCII" << "Hex";
        */

        tItem = new QTableWidgetItem(QString::number(tempPacket.port));
        Packet::populateTableWidgetItem(tItem, tempPacket);
        ui->trafficLogTable->setItem(rowCounter, packetTableHeaders.indexOf("To Port"), tItem);

        tItem = new QTableWidgetItem(tempPacket.tcpOrUdp);
        Packet::populateTableWidgetItem(tItem, tempPacket);
        ui->trafficLogTable->setItem(rowCounter, packetTableHeaders.indexOf("Method"), tItem);

        tItem = new QTableWidgetItem(tempPacket.errorString);
        Packet::populateTableWidgetItem(tItem, tempPacket);
        ui->trafficLogTable->setItem(rowCounter, packetTableHeaders.indexOf("Error"), tItem);

        tItem = new QTableWidgetItem(tempPacket.hexToASCII(tempPacket.hexString));
        QSize tSize = tItem->sizeHint();
        tSize.setWidth(200);
        tItem->setSizeHint(tSize);
        Packet::populateTableWidgetItem(tItem, tempPacket);
        ui->trafficLogTable->setItem(rowCounter, packetTableHeaders.indexOf("ASCII"), tItem);

        tItem = new QTableWidgetItem(tempPacket.hexString);
        Packet::populateTableWidgetItem(tItem, tempPacket);
        ui->trafficLogTable->setItem(rowCounter, packetTableHeaders.indexOf("Hex"), tItem);

        rowCounter++;



    }
    ui->trafficLogTable->setSortingEnabled(true);

    ui->trafficLogTable->resizeColumnsToContents();
    ui->trafficLogTable->resizeRowsToContents();
    ui->trafficLogTable->horizontalHeader()->setStretchLastSection( true );

}



void MainWindow::loadPacketsTable()
{
    QSettings settings(SETTINGSFILE, QSettings::IniFormat);

    tableActive = false;
    Packet tempPacket;


    QList<Packet> packetsSavedFiltered;
    packetsSavedFiltered.clear();

    QString filterString = ui->searchLineEdit->text().toLower().trimmed();


    foreach(tempPacket, packetsSaved)
    {

        if(tempPacket.name.toLower().contains(filterString) ||
                tempPacket.hexToASCII(tempPacket.hexString).toLower().contains(filterString) ||
                tempPacket.toIP.toLower().contains(filterString) ||
                QString::number(tempPacket.port).contains(filterString)
                )
        {
            packetsSavedFiltered.append(tempPacket);
            continue;
        }
    }

    QDEBUGVAR(packetsSavedFiltered.size());



    ui->packetsTable->clear();



    packetSavedTableHeaders  = Settings::defaultPacketTableHeader();
    packetSavedTableHeaders = settings.value("packetSavedTableHeaders", packetSavedTableHeaders).toStringList();

    ui->packetsTable->setColumnCount(packetSavedTableHeaders.size());

    ui->packetsTable->verticalHeader()->show();
    ui->packetsTable->horizontalHeader()->show();
    ui->packetsTable->setHorizontalHeaderLabels(packetSavedTableHeaders);
    if(packetsSavedFiltered.isEmpty())
    {
        ui->packetsTable->setRowCount(0);
    } else {
        ui->packetsTable->setRowCount(packetsSavedFiltered.count());
    }

    unsigned int rowCounter = 0;
    foreach(tempPacket, packetsSavedFiltered)
    {
        populateTableRow(rowCounter, tempPacket);
        rowCounter++;
    }



    ui->packetsTable->resizeColumnsToContents();
    ui->packetsTable->resizeRowsToContents();
    ui->packetsTable->horizontalHeader()->setStretchLastSection( true );

    tableActive = true;


}

void MainWindow::httpFinished(QNetworkReply* pReply)
{

    QByteArray data=pReply->readAll();
    QString str = QString(data);
    str.truncate(100);
    QDEBUG() <<"finished http. size="<<data.size() << str;

}


MainWindow::~MainWindow()
{
    delete ui;
}

quint64 MainWindow::timeSinceLaunch()
{
    static QTime launchTimer = QTime::currentTime();
    if(launchTimer.isValid())
    {
        return launchTimer.elapsed();

    } else {
        launchTimer.start();
        return 0;
    }

}

void MainWindow::on_packetHexEdit_lostFocus()
{

    QString quicktestHex =  ui->packetHexEdit->text();

    ui->packetASCIIEdit->setText(Packet::hexToASCII(quicktestHex));
    ui->packetASCIIEdit->setToolTip("");
    ui->packetHexEdit->setText(quicktestHex);


}

void MainWindow::on_packetASCIIEdit_lostFocus()
{

    QString quicktestASCII =  ui->packetASCIIEdit->text();
    ui->packetHexEdit->setText( Packet::ASCIITohex(quicktestASCII));


    QString quicktestASCII2 =  ui->packetHexEdit->text();

    ui->packetASCIIEdit->setText(Packet::hexToASCII(quicktestASCII2));
    ui->packetASCIIEdit->setToolTip("");

    qDebug() << __FILE__ << "/" << __LINE__ <<"on_serialASCIIEdit_lostFocus";

}

void MainWindow::statusBarMessage(const QString &msg, int timeout = 3000, bool override = false)
{
    QString currentMsg = statusBar()->currentMessage();

    if(currentMsg.size() > 10)
    {
        override = true;
    }
    if(currentMsg.size() > 0 && !override)
    {
        statusBar()->showMessage(currentMsg + " / " + msg, timeout * 2);
    } else {
        if (timeout == 0)
            timeout = 3000;
        statusBar()->showMessage(msg, timeout);
    }
}
void MainWindow::sendClick(QString packetName)
{
    QDEBUG() << "send click: " << packetName;
    Packet toSend;


    foreach(toSend, packetsSaved)
    {
        if(toSend.name == packetName)
        {

            if(toSend.toIP.trimmed() == "255.255.255.255") {

                QSettings settings(SETTINGSFILE, QSettings::IniFormat);
                bool sendResponse = settings.value("sendReponse", false).toBool();
                if(sendResponse) {
                    QMessageBox msgBox;
                    msgBox.setWindowTitle("Broadcast with responses!");
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    msgBox.setDefaultButton(QMessageBox::No);
                    msgBox.setIcon(QMessageBox::Warning);
                    msgBox.setText("You are sending a broadcast packet with responses enabled.\n\nThis could cause traffic flooding. Continue?");
                    int yesno = msgBox.exec();
                    if(yesno == QMessageBox::No) {
                        return;
                    }

                }
            }

            if(toSend.repeat > 0 && !ui->persistentTCPCheck->isChecked())
            {
                toSend.timestamp = QDateTime::currentDateTime();

                stopResendingButton->setStyleSheet("QPushButton { color: green; } QPushButton::hover { color: #BC810C; } ");
                packetsRepeat.append(toSend);
                stopResendingButton->setText("Resending (" + QString::number(packetsRepeat.size()) + ")");
                stopResending = 0;
            }

            lastSendPacket = toSend;
            QByteArray sendData = toSend.getByteArray();
            statusBarMessage("Send: " + packetName + "  (" + QString::number(sendData.size()) + " bytes)");
            emit sendPacket(toSend);
        }
    }

}

void MainWindow::on_savePacketButton_clicked()
{

    Packet testPacket;
    testPacket.init();
    testPacket.name = ui->packetNameEdit->text().trimmed();
    if(testPacket.name.isEmpty()) {

        QMessageBox msgBox;
        msgBox.setText("Name cannot be blank.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Name is empty.");
        msgBox.exec();
        ui->packetNameEdit->setFocus();
        return;

    }
    testPacket.toIP = ui->packetIPEdit->text().trimmed();
    testPacket.hexString =  ui->packetHexEdit->text().simplified();
    testPacket.tcpOrUdp = ui->udptcpComboBox->currentText();
    testPacket.sendResponse =  0;
    testPacket.port = ui->packetPortEdit->text().toUInt();
    testPacket.repeat = Packet::oneDecimal(ui->resendEdit->text().toFloat());

    testPacket.saveToDB();
    packetsSaved = Packet::fetchAllfromDB("");

    //ui->searchLineEdit->setText("");
    loadPacketsTable();

}

void MainWindow::saveSession(Packet sessionPacket)
{

    QSettings settings(SETTINGSFILE, QSettings::IniFormat);

    //settings.setValue("packetNameEditSession", ui->packetNameEdit->text());
    settings.setValue("packetIPEditSession", ui->packetIPEdit->text());
    settings.setValue("packetHexEditSession", ui->packetHexEdit->text());
    settings.setValue("udptcpComboBoxSession", ui->udptcpComboBox->currentText());
    settings.setValue("packetPortEditSession", ui->packetPortEdit->text());
    settings.setValue("resendEditSession", ui->resendEdit->text());

}


void MainWindow::on_testPacketButton_clicked()
{
    Packet testPacket;
    testPacket.init();

    if(ui->udptcpComboBox->currentText() == "SSL") {

        if(!QSslSocket::supportsSsl()) {

            QMessageBox msgBox;
            msgBox.setText("This computer does not support SSL.\n\nExpected SSL:" + QSslSocket::sslLibraryVersionString() );
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setWindowTitle("No SSL Library.");
            msgBox.exec();
            ui->udptcpComboBox->setFocus();
            return;

        }
    }

    if(ui->testPacketButton->text().contains("Multi")) {
        QDEBUG() << "We are multi";
        int packetCount = 0;

        QList<QTableWidgetItem *> totalSelected = ui->packetsTable->selectedItems();
        if(!totalSelected.isEmpty()) {
            QTableWidgetItem * item;
            QList<int> usedRows;
            usedRows.clear();
            foreach(item, totalSelected) {
                if(usedRows.contains(item->row())) {
                    continue;
                } else {
                    usedRows.append(item->row());
                    Packet clickedPacket = Packet::fetchTableWidgetItemData(item);

                    emit sendPacket(clickedPacket);
                    packetCount++;

                }

            }

        }
        statusBarMessage("Sending "+QString::number(packetCount)+" packets");
        return;

    }


    testPacket.name = ui->packetNameEdit->text().trimmed();
    testPacket.toIP = ui->packetIPEdit->text().trimmed();
    testPacket.hexString =  ui->packetHexEdit->text().simplified();
    testPacket.tcpOrUdp = ui->udptcpComboBox->currentText();
    testPacket.sendResponse =  0;
    testPacket.port = ui->packetPortEdit->text().toUInt();
    testPacket.repeat = Packet::oneDecimal(ui->resendEdit->text().toFloat());

    //Save Session!
    saveSession(testPacket);

    if(testPacket.toIP.isEmpty()) {

        QMessageBox msgBox;
        msgBox.setText("Address cannot be blank.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Address is empty.");
        msgBox.exec();
        ui->packetIPEdit->setFocus();
        return;

    }


    if(testPacket.port == 0) {

        QMessageBox msgBox;
        msgBox.setText("Port cannot be blank/zero.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Port is zero.");
        msgBox.exec();
        ui->packetPortEdit->setFocus();
        return;

    }

    if(testPacket.toIP.trimmed() == "255.255.255.255") {

        QSettings settings(SETTINGSFILE, QSettings::IniFormat);
        bool sendResponse = settings.value("sendReponse", false).toBool();
        if(sendResponse) {

            QMessageBox msgBox;
            msgBox.setWindowTitle("Broadcast with responses!");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::No);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("You are sending a broadcast packet with responses enabled.\n\nThis could cause traffic flooding. Continue?");
            int yesno = msgBox.exec();
            if(yesno == QMessageBox::No) {
                return;
            }
        }
    }


    lastSendPacket = testPacket;
    lastSendPacket.name.clear();

    if(testPacket.repeat > 0 && !ui->persistentTCPCheck->isChecked())
    {
        /*
        if(testPacket.tcpOrUdp == "TCP" && ui->persistentTCPCheck->isChecked()) {

            QMessageBox msgBox;
            msgBox.setWindowTitle("Resend TCP with persistent connections!");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Yes);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("You are resending a TCP packet with presistent connecitons. The UI could spawn numerous windows! \n\nUncheck persistent connection? (Recommended)");
            int yesno = msgBox.exec();
            if(yesno == QMessageBox::Yes) {
                ui->persistentTCPCheck->setChecked(false);
                on_persistentTCPCheck_clicked(false);
            }
            if(yesno == QMessageBox::Cancel) {
                return;
            }

        }
            */

        testPacket.timestamp = QDateTime::currentDateTime();

        stopResendingButton->setStyleSheet("QPushButton { color: green; } QPushButton::hover { color: #BC810C; } ");
        packetsRepeat.append(testPacket);
        stopResendingButton->setText("Resending (" + QString::number(packetsRepeat.size()) + ")");
        stopResending = 0;
    }

    emit sendPacket(testPacket);


}

void MainWindow::on_deletePacketButton_clicked()
{
    QModelIndexList indexes = ui->packetsTable->selectionModel()->selectedIndexes();
    QDEBUG() << "Delete packets";
    QModelIndex index;
    QString selected;

    if(indexes.isEmpty()) {

        QMessageBox msgBox;
        msgBox.setText("No packets selected.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle("Select a packet.");
        msgBox.exec();
        return;
    }

    foreach(index, indexes)
    {
        selected = index.data(Packet::PACKET_NAME).toString();
        QDEBUG() << "Deleting"<<selected;
        Packet::removeFromDB(selected);
    }

    packetsSaved = Packet::fetchAllfromDB("");
    ui->searchLineEdit->setText("");
    loadPacketsTable();

}



void MainWindow::on_packetIPEdit_lostFocus()
{
    QString ipPacket = ui->packetIPEdit->text().trimmed();
    QHostAddress address(ipPacket);
    if (QAbstractSocket::IPv4Protocol == address.protocol())
    {
       QDEBUG() << "Valid IPv4 address.";
    }
    else if (QAbstractSocket::IPv6Protocol == address.protocol())
    {
       QDEBUG() << "Valid IPv6 address.";
    }
    else
    {
        QHostInfo info = QHostInfo::fromName(ipPacket);
        if (info.error() != QHostInfo::NoError)
        {
            ui->packetIPEdit->setText("");
            ui->packetIPEdit->setPlaceholderText("Invalid Address / DNS failed");
        } else {

            QSettings settings(SETTINGSFILE, QSettings::IniFormat);

            if(settings.value("resolveDNSOnInputCheck", false).toBool()) {
                ui->packetIPEdit->setText(info.addresses().at(0).toString());
            } else {
                statusBarMessage(ipPacket + " --> " + info.addresses().at(0).toString());
            }
        }
    }
}

void MainWindow::on_packetPortEdit_lostFocus()
{
    QString portPacket = ui->packetPortEdit->text().trimmed();
    unsigned int port =0 ;
    bool ok;
    port = portPacket.toUInt(&ok, 0);
    QDEBUGVAR(port);
    if(port <= 0 )
    {
        ui->packetPortEdit->setText("");
        ui->packetPortEdit->setPlaceholderText("Invalid Port");
    } else {
        ui->packetPortEdit->setText(QString::number(port));
    }
}

 //   QDEBUG() << "cell changed";

void MainWindow::removePacketfromMemory(Packet thepacket)
{
    for(int i=0; i<packetsSaved.size();i++)
    {
        if(thepacket.name == packetsSaved[i].name)
        {
            packetsSaved.removeAt(i);
            break;
        }

    }

}

void MainWindow::on_packetsTable_itemChanged(QTableWidgetItem *item)
{
    if(!tableActive)
    {
        return;
    }
    tableActive =  false;
    QString datatype = item->data(Packet::DATATYPE).toString();
    QString newText = item->text();
    QDEBUG() << "cell changed:" <<datatype << item->text();
    int fullrefresh = 0;

    Packet updatePacket = Packet::fetchTableWidgetItemData(item);
    if(datatype == "name")
    {
        Packet::removeFromDB(updatePacket.name); //remove old before inserting new.
        removePacketfromMemory(updatePacket);
        updatePacket.name = newText;
        fullrefresh = 1;
    }
    if(datatype == "toIP")
    {
        QHostAddress address(newText);
        if (QAbstractSocket::IPv4Protocol == address.protocol())
        {
            updatePacket.toIP = newText;
        }        else if (QAbstractSocket::IPv6Protocol == address.protocol())
        {
            updatePacket.toIP = newText;
        } else {
            QHostInfo info = QHostInfo::fromName(newText);
            if (info.error() == QHostInfo::NoError)
            {

                updatePacket.toIP = newText;

                QSettings settings(SETTINGSFILE, QSettings::IniFormat);

                if(settings.value("resolveDNSOnInputCheck", false).toBool()) {
                    updatePacket.toIP = (info.addresses().at(0).toString());
                } else {
                    statusBarMessage(newText + " --> " + info.addresses().at(0).toString());
                }            }

        }


    }
    if(datatype == "port")
    {
        int portNum = newText.toUInt();
        if(portNum > 0)
        {
            updatePacket.port = portNum;
        }
    }
    if(datatype == "repeat")
    {
        float repeat = Packet::oneDecimal(newText.toFloat());
        updatePacket.repeat = repeat;
    }
    if(datatype == "tcpOrUdp")
    {
        if((newText.trimmed().toUpper() == "TCP") || (newText.trimmed().toUpper() == "UDP") || (newText.trimmed().toUpper() == "SSL" ))
        {
            updatePacket.tcpOrUdp = newText.trimmed().toUpper();
        }
    }
    if(datatype == "ascii")
    {
        QString hex = Packet::ASCIITohex(newText);
        updatePacket.hexString = hex;
    }
    if(datatype == "hexString")
    {
        QString hex = newText;
        QString ascii = Packet::hexToASCII(newText);
        updatePacket.hexString = newText;
    }

    updatePacket.saveToDB();
    packetsSaved = Packet::fetchAllfromDB("");

    if(fullrefresh)
    {
        loadPacketsTable();
    } else {
        populateTableRow(item->row(), updatePacket);

    }
    tableActive =  true;
}



void MainWindow::poodlepic()
{
    QDEBUG();

    BruceThePoodle *bruce = new BruceThePoodle(this);
    bruce->show();
}

void MainWindow::shortcutkey1() { ui->packetNameEdit->setFocus(); }
void MainWindow::shortcutkey2() { ui->packetASCIIEdit->setFocus(); }
void MainWindow::shortcutkey3() { ui->packetHexEdit->setFocus(); }
void MainWindow::shortcutkey4() { ui->packetIPEdit->setFocus(); }
void MainWindow::shortcutkey5() { ui->packetPortEdit->setFocus(); }
void MainWindow::shortcutkey6() { ui->resendEdit->setFocus(); }
void MainWindow::shortcutkey7() { ui->udptcpComboBox->setFocus(); }


//packetSavedTableHeaders <<"Send" <<"Resend (s)"<< "Name" << "To Address" << "To Port" << "Method" << "ASCII" << "Hex";


int MainWindow::findColumnIndex(QListWidget * lw, QString search)
{
    QListWidgetItem * item;
    QString text;
    int size = lw->count();

    for(int i = 0; i < size; i++)
    {
        item = lw->item(i);
        text = item->text();
        if(text == search)
        {
            return i;
        }

    }
    QDEBUGVAR(search);
    return -1;

}

void MainWindow::populateTableRow(int rowCounter, Packet tempPacket)
{
    QTableWidgetItem * tItem;

    SendPacketButton * sendButton = tempPacket.getSendButton(ui->packetsTable);


    connect(sendButton, SIGNAL(sendPacket(QString)),
        this, SLOT(sendClick(QString)));

    //http->get(QNetworkRequest(QUrl("http://packetsender.com/")));

    /*
     *
     *
    packetSavedTableHeaders <<"Send" <<"Resend (sec)"<< "Name" << "To Address" <<
                "To Port" << "Method" << "ASCII" << "Hex";
    packetTableHeaders.clear();
    packetTableHeaders <<"Time" << "From IP" <<"From Port" << "To IP" << "To Port" << "Method"
                <<"Error" << "ASCII" << "Hex";

     */


    ui->packetsTable->setCellWidget(rowCounter,packetSavedTableHeaders.indexOf("Send"), sendButton);

    tItem = new QTableWidgetItem(QString::number(tempPacket.repeat));
    Packet::populateTableWidgetItem(tItem, tempPacket);
    tItem->setData(Packet::DATATYPE, "repeat");
    ui->packetsTable->setItem(rowCounter, packetSavedTableHeaders.indexOf("Resend (sec)"), tItem);
    //QDEBUGVAR(tempPacket.name);

    tItem = new QTableWidgetItem(tempPacket.name);
    Packet::populateTableWidgetItem(tItem, tempPacket);
    tItem->setData(Packet::DATATYPE, "name");
    ui->packetsTable->setItem(rowCounter, packetSavedTableHeaders.indexOf("Name"), tItem);
    //QDEBUGVAR(tempPacket.name);

    tItem = new QTableWidgetItem(tempPacket.toIP);
    Packet::populateTableWidgetItem(tItem, tempPacket);
    tItem->setData(Packet::DATATYPE, "toIP");

    ui->packetsTable->setItem(rowCounter, packetSavedTableHeaders.indexOf("To Address"), tItem);
    //QDEBUGVAR(tempPacket.toIP);

    tItem = new QTableWidgetItem(QString::number(tempPacket.port));
    Packet::populateTableWidgetItem(tItem, tempPacket);
    tItem->setData(Packet::DATATYPE, "port");
    ui->packetsTable->setItem(rowCounter, packetSavedTableHeaders.indexOf("To Port"), tItem);
   // QDEBUGVAR(tempPacket.port);

    tItem = new QTableWidgetItem(tempPacket.tcpOrUdp);
    Packet::populateTableWidgetItem(tItem, tempPacket);
    tItem->setData(Packet::DATATYPE, "tcpOrUdp");
    ui->packetsTable->setItem(rowCounter, packetSavedTableHeaders.indexOf("Method"), tItem);
   // QDEBUGVAR(tempPacket.tcpOrUdp);

    tItem = new QTableWidgetItem(tempPacket.hexToASCII(tempPacket.hexString));
    Packet::populateTableWidgetItem(tItem, tempPacket);
    tItem->setData(Packet::DATATYPE, "ascii");

    QSize tSize = tItem->sizeHint();
    tSize.setWidth(200);
    tItem->setSizeHint(tSize);

    ui->packetsTable->setItem(rowCounter, packetSavedTableHeaders.indexOf("ASCII"), tItem);
    //QDEBUGVAR(tempPacket.hexString);

    tItem = new QTableWidgetItem(tempPacket.hexString);
    Packet::populateTableWidgetItem(tItem, tempPacket);
    tItem->setData(Packet::DATATYPE, "hexString");
    ui->packetsTable->setItem(rowCounter, packetSavedTableHeaders.indexOf("Hex"), tItem);
    //QDEBUGVAR(tempPacket.hexString);
}

void clearLayout(QLayout* layout, bool deleteWidgets = true)
{
    while (QLayoutItem* item = layout->takeAt(0))
    {
        if (deleteWidgets)
        {
            if (QWidget* widget = item->widget())
                delete widget;
        }
        if (QLayout* childLayout = item->layout())
            clearLayout(childLayout, deleteWidgets);
        delete item;
    }
}

void MainWindow::packetTable_checkMultiSelected() {

    //how many are selected?
    QTableWidgetItem * checkItem;
    QList<Packet> packetList;
    Packet clickedPacket;
    QList<QTableWidgetItem *> totalSelected = ui->packetsTable->selectedItems();
    packetList.clear();
    QStringList buttonsList;
    buttonsList.clear();


    foreach(checkItem, totalSelected) {
        clickedPacket = Packet::fetchTableWidgetItemData(checkItem);
        if(buttonsList.contains(clickedPacket.name)) {
            continue;
        } else {
            packetList.append(clickedPacket);
            buttonsList.append(clickedPacket.name);
        }
    }


    if(packetList.size() >= 2) {
        //We have multi!
        ui->testPacketButton->setText("Multi-Send");
        ui->testPacketButton->setStyleSheet("color:green;");
        ui->packetASCIIEdit->setEnabled(false);
        ui->packetNameEdit->setEnabled(false);
        ui->packetHexEdit->setEnabled(false);
        ui->packetIPEdit->setEnabled(false);
        ui->packetPortEdit->setEnabled(false);
        ui->udptcpComboBox->setEnabled(false);
        ui->savePacketButton->setEnabled(false);
        ui->resendEdit->setEnabled(false);
        return;

    }


    ui->testPacketButton->setText("Send");
    ui->testPacketButton->setStyleSheet("");

    ui->packetNameEdit->setEnabled(true);
    ui->packetASCIIEdit->setEnabled(true);
    ui->packetHexEdit->setEnabled(true);
    ui->packetIPEdit->setEnabled(true);
    ui->packetPortEdit->setEnabled(true);
    ui->udptcpComboBox->setEnabled(true);
    ui->savePacketButton->setEnabled(true);
    ui->resendEdit->setEnabled(true);


}

void MainWindow::on_packetsTable_itemClicked(QTableWidgetItem *item)
{


    packetTable_checkMultiSelected();


    if(item->isSelected())
    {
        Packet clickedPacket = Packet::fetchTableWidgetItemData(item);

        if(item->column() != 0)
        {
            ui->packetNameEdit->setText(clickedPacket.name);
            ui->packetHexEdit->setText(clickedPacket.hexString);
            ui->packetIPEdit->setText(clickedPacket.toIP);
            ui->packetPortEdit->setText(QString::number(clickedPacket.port));
            ui->resendEdit->setText(QString::number(clickedPacket.repeat));
            ui->udptcpComboBox->setCurrentIndex(ui->udptcpComboBox->findText(clickedPacket.tcpOrUdp));
            ui->packetASCIIEdit->setText(Packet::hexToASCII(clickedPacket.hexString));
            ui->packetASCIIEdit->setToolTip("");

        }
    }
}


void MainWindow::toggleIPv4_IPv6()
{
    QString currentMode = IPmodeButton->text();
    if(currentMode.contains("4")) {
        packetNetwork.setIPmode(6);
    } else {
        packetNetwork.setIPmode(4);
    }

    setIPMode();

    applyNetworkSettings();


}

void MainWindow::refreshTimerTimeout()
{

    QDateTime now = QDateTime::currentDateTime();

    for(int i = 0; i < packetsRepeat.size() && !stopResending; i++)
    {

        int repeatMS = (int) (packetsRepeat[i].repeat * 1000 - 100);

        if(packetsRepeat[i].timestamp.addMSecs(repeatMS) < now)
        {
            packetsRepeat[i].timestamp = now;

            if(((resendCounter+1) < cancelResendNum) || (cancelResendNum == 0)) {

                emit sendPacket(packetsRepeat[i]);
                statusBarMessage("Send: " + packetsRepeat[i].name + " (Resend)");
                resendCounter++;

            } else {
                    stopResending = 1;
            }

        }
    }


    if(packetsRepeat.isEmpty() || stopResending)
    {
        stopResendingButton->hide();
        packetsRepeat.clear();
        stopResending = 0;
        resendCounter = 0;

    } else {
        stopResendingButton->show();
    }


    if(ui->trafficLogTable->rowCount() != packetsLogged.size())
    {
        while(maxLogSize > 0 && packetsLogged.size() > maxLogSize) {
            packetsLogged.removeFirst();
        }

        loadTrafficLogTable();

        //ui->mainTabWidget->setTabText(1,"Traffic Log (" + QString::number(packetsLogged.size()) +")");

    }


}

void MainWindow::on_trafficLogClearButton_clicked()
{
    ui->trafficLogTable->clear();
    ui->trafficLogTable->setRowCount(0);
    packetsLogged.clear();
    loadTrafficLogTable();

}


void MainWindow::on_saveTrafficPacket_clicked()
{
    QList<QTableWidgetItem *> items = ui->trafficLogTable->selectedItems();
    QDEBUG() << "Save traffic packets";
    QTableWidgetItem *item;
    QString selected;


    Packet savePacket;
    QString namePrompt;
    bool ok;
    foreach(item, items)
    {
        savePacket = Packet::fetchTableWidgetItemData(item);
        QDEBUG() << "Saving"<<savePacket.name;
        namePrompt = QInputDialog::getText(this, tr("Save Packet"),
                                                  tr("Packet name:"), QLineEdit::Normal,
                                                  savePacket.name, &ok);
        if (ok && !namePrompt.isEmpty())
        {
            savePacket.name = namePrompt.trimmed();
        }


        if(savePacket.toIP.toUpper().trimmed() == "YOU")
        {
            savePacket.toIP = savePacket.fromIP;
            savePacket.port = savePacket.fromPort;
        }

        savePacket.repeat = 0;

        savePacket.saveToDB();
        break;
    }

    packetsSaved = Packet::fetchAllfromDB("");
    ui->searchLineEdit->setText("");
    loadPacketsTable();


}
void MainWindow::applyNetworkSettings()
{
    QSettings settings(SETTINGSFILE, QSettings::IniFormat);

    packetsRepeat.clear();

    multiSendDelay = settings.value("multiSendDelay", 0).toFloat();
    cancelResendNum = settings.value("cancelResendNum", 0).toInt();
    resendCounter = 0;


    asciiEditTranslateEBCDIC = settings.value("asciiEditTranslateEBCDICCheck", false).toBool();


    packetNetwork.kill();
    packetNetwork.init();
    packetNetwork.responseData = settings.value("responseHex","").toString().trimmed();
    packetNetwork.sendResponse = settings.value("sendReponse", false).toBool();
    ui->persistentTCPCheck->setChecked(settings.value("persistentTCPCheck", false).toBool());
    on_persistentTCPCheck_clicked(ui->persistentTCPCheck->isChecked());

    UDPServerStatus();
    TCPServerStatus();

}

void MainWindow::cancelResends()
{
    stopResendingButton->setStyleSheet("QPushButton { color: black; } QPushButton::hover { color: #BC810C; } ");
    stopResendingButton->setText("Resending");
    stopResending = 1;
}


void MainWindow::on_packetASCIIEdit_editingFinished()
{
    on_packetASCIIEdit_lostFocus();
}

void MainWindow::on_packetHexEdit_editingFinished()
{
    on_packetHexEdit_lostFocus();
}

void MainWindow::on_packetASCIIEdit_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1);

}

void MainWindow::on_packetIPEdit_editingFinished()
{
    on_packetIPEdit_lostFocus();
}



void MainWindow::on_searchLineEdit_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1);

    loadPacketsTable();
    loadTrafficLogTable();

}


void MainWindow::on_toClipboardButton_clicked()
{
    QSettings settings(SETTINGSFILE, QSettings::IniFormat);


    QClipboard *clipboard = QApplication::clipboard();
    QList<QTableWidgetItem *> items = ui->trafficLogTable->selectedItems();
    QDEBUG() << "Save traffic packets";
    QTableWidgetItem *item;
    QString selected;


    Packet savePacket;
    QString clipString;
    clipString.clear();
    QTextStream out;
    out.setString(&clipString);

    if(items.size() == 0)
    {

        QMessageBox msgBox;
        msgBox.setText("No packets selected.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle("Clipboard unchanged.");
        msgBox.exec();
        return;
    }
    foreach(item, items)
    {
        savePacket = Packet::fetchTableWidgetItemData(item);
        out << "Time: " << savePacket.timestamp.toString(DATETIMEFORMAT) << "\n";
        out << "TO: " << savePacket.toIP << ":" << savePacket.port<< "\n";
        out << "From: " << savePacket.fromIP << ":" << savePacket.fromPort << "\n";
        out << "Method: " << savePacket.tcpOrUdp << "\n";
        out << "Error: " << savePacket.errorString;
        out <<"\n\nASCII:"<<"\n";
        out << savePacket.hexToASCII(savePacket.hexString) << "\n";
        out <<"\n\nHEX:"<<"\n";
        out << savePacket.hexString << "\n";

        break;

    }

    QMessageBox msgBox;

    if(settings.value("copyUnformattedCheck", true).toBool()) {
        clipboard->setText(QString(savePacket.getByteArray()));
        msgBox.setText("Raw packet data sent to your clipboard. \nChange the settings if you prefer translated data.");
    } else {
        clipboard->setText(clipString);
        msgBox.setText("Packet sent to your clipboard (translated).");
    }


    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle("To Clipboard.");
    msgBox.exec();


}



void MainWindow::on_packetsTable_itemSelectionChanged()
{
    packetTable_checkMultiSelected();

}


void MainWindow::on_bugsLinkButton_clicked()
{
    QDesktopServices::openUrl(QUrl("http://bugtracker.dannagle.com/"));
}


void MainWindow::on_forumsPacketSenderButton_clicked()
{
    QDesktopServices::openUrl(QUrl("http://forums.packetsender.com/"));


}


void MainWindow::on_saveLogButton_clicked()
{
    static QString fileName = QDir::homePath() + QString("/trafficlog.log");

    fileName = QFileDialog::getSaveFileName(this, tr("Save Traffic Log"),
                               QDir::toNativeSeparators(fileName), tr("log (*.log)"));

   QStringList testExt = fileName.split(".");
   QString ext = "";
   if(testExt.size() > 0) {
       ext = testExt.last();
   }
   if(ext != "log") {
       fileName.append(".log");
   }
   QDEBUG() << "Export log to" << fileName;

   QString exportString = "";
   QString delim = "\t";

   QTextStream out;
   out.setString(&exportString);

   out << "TIME" << delim << "From IP" << delim << "From Port" << delim << "To IP"
       << delim << "To Port" << delim << "Method" << delim << "Error" << delim << "ASCII" << delim << "Hex\n";


   Packet tempPacket;
   foreach(tempPacket, packetsLogged)
   {

       exportString.append(tempPacket.timestamp.toString(DATETIMEFORMAT));exportString.append(delim);
       exportString.append(tempPacket.fromIP);exportString.append(delim);
       exportString.append(QString::number(tempPacket.fromPort));exportString.append(delim);
       exportString.append(tempPacket.toIP);exportString.append(delim);
       exportString.append(QString::number(tempPacket.port));exportString.append(delim);
       exportString.append(tempPacket.tcpOrUdp);exportString.append(delim);
       exportString.append(tempPacket.errorString);exportString.append(delim);
       exportString.append(tempPacket.hexToASCII(tempPacket.hexString));exportString.append(delim);
       exportString.append(tempPacket.hexString);exportString.append(delim);
       exportString.append("\n");
   }


   QFile file(fileName);
   if(file.open(QFile::WriteOnly)) {
       file.write(exportString.toLatin1());
       file.close();
   }

   statusBarMessage("Save Log: " + fileName);

}

void MainWindow::setIPMode() {

    int ipMode = packetNetwork.getIPmode();

    if(ipMode > 4) {
        IPmodeButton->setText("IPv6 Mode");
        IPmodeButton->setStyleSheet(IPv6Stylesheet);
    } else {
        IPmodeButton->setText("IPv4 Mode");
        IPmodeButton->setStyleSheet(IPv4Stylesheet);
    }


}

void MainWindow::on_actionAbout_triggered()
{
    About * about = new About(this);
    about->show();
}

void MainWindow::on_actionHelp_triggered()
{
    //Open URL in browser
    QDesktopServices::openUrl(QUrl("https://packetsender.com/documentation"));
}

void MainWindow::on_actionSettings_triggered()
{
    Settings settings;
    int accepted = settings.exec();
    if(accepted) {
        setIPMode();
        applyNetworkSettings();
        loadPacketsTable();
        loadTrafficLogTable();
    }
}

void MainWindow::on_actionExit_triggered()
{
    QDEBUG();
    exit(0);
}

void MainWindow::on_actionImport_Packets_triggered()
{

    static QString fileName = QDir::homePath() + QString("/packetsender_export.ini");


    fileName = QFileDialog::getOpenFileName(this, tr("Import File"),
                                              fileName,
                                              tr("INI db (*.ini)"));

    QDEBUGVAR(fileName);

    if(fileName.isEmpty()) {
        return;
    }

    QList<Packet> importList = Packet::fetchAllfromDB(fileName);
    Packet importPacket;

    foreach(importPacket, importList) {
        QDEBUGVAR(importPacket.name);
    }

    if(!importList.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Found " + QString::number(importList.size()) + " packets!");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText("Import " + QString::number(importList.size()) + " packets?\n\nPacket Sender will overwrite packets with the same name.");
        int yesno = msgBox.exec();
        if(yesno == QMessageBox::No) {
            statusBarMessage("Import Cancelled");
            return;
        } else {
            foreach(importPacket, importList) {
                importPacket.saveToDB();

            }
            packetsSaved = Packet::fetchAllfromDB("");
            statusBarMessage("Import Finished");
            loadPacketsTable();
        }

    } else {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Not a database");
        msgBox.setStandardButtons(QMessageBox::Ok );
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Found no packets in this file. It may not be a Packet Sender export");
        msgBox.exec();
        return;
        statusBarMessage("Import Cancelled");
    }
}

void MainWindow::on_actionExport_Packets_triggered()
{

    static QString fileName = QDir::homePath() + QString("/packetsender_export.ini");

    fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               QDir::toNativeSeparators(fileName), tr("INI db (*.ini)"));


    if(fileName.isEmpty()) {
        return;
    }

   QStringList testExt = fileName.split(".");
   QString ext = "";
   if(testExt.size() > 0) {
       ext = testExt.last();
   }
   if(ext != "ini") {
       fileName.append(".ini");
   }
   QDEBUGVAR(fileName);
   QFile::copy(PACKETSFILE, fileName);
   statusBarMessage("Export: " + fileName);
}

void MainWindow::on_persistentTCPCheck_clicked(bool checked)
{
    QSettings settings(SETTINGSFILE, QSettings::IniFormat);
    settings.setValue("persistentTCPCheck", ui->persistentTCPCheck->isChecked());

    packetNetwork.persistentConnectCheck = checked;
}

void MainWindow::on_actionSubnet_Calculator_triggered()
{
    QDEBUG();
    SubnetCalc * sCalc = new SubnetCalc(this);
    sCalc->show();

}

void MainWindow::on_resendEdit_editingFinished()
{
    float resendVal = Packet::oneDecimal(ui->resendEdit->text().toFloat());
    ui->resendEdit->setText(QString::number(resendVal));
}

void MainWindow::on_loadFileButton_clicked()
{
    static QString fileName;
    static bool showWarning = true;

    if(fileName.isEmpty()) {
        fileName = QDir::homePath();
    }

    fileName = QFileDialog::getOpenFileName(this, tr("Import File"),
                                              fileName,
                                              tr("*.*"));

    QDEBUGVAR(fileName);

    if(fileName.isEmpty()) {
        return;
    }

    QFile loadFile(fileName);

    if(!loadFile.exists()) {
        return;
    }

    QByteArray data;
    if(loadFile.open(QFile::ReadOnly)) {
        data = loadFile.readAll();
        loadFile.close();
        if(data.size() > (32767 / 3)) {
            data.resize(32767 / 3);

            if(showWarning) {
                showWarning = false;
                QMessageBox msgBox;
                msgBox.setWindowTitle("Max size exceeded!");
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setText("The hex field supports up to 10,922 bytes. The data has been truncated.");
                msgBox.exec();

            }

        }
        statusBarMessage("Loading " + QString::number(data.size()) + " bytes");
        ui->packetHexEdit->setText(Packet::byteArrayToHex(data));
        on_packetHexEdit_lostFocus();
        on_packetASCIIEdit_lostFocus();
        ui->packetASCIIEdit->setFocus();
        QDEBUGVAR(ui->packetHexEdit->text().size());
    }


}

void MainWindow::on_actionDonate_Thank_You_triggered()
{

    //Open URL in browser
    QDesktopServices::openUrl(QUrl("http://dannagle.com/paypal"));
}
