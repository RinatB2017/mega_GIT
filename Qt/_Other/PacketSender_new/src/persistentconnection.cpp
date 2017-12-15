#include "persistentconnection.h"
#include "ui_persistentconnection.h"

#include <QTextStream>
#include <QScrollBar>
#include <QSettings>
#include <QDesktopServices>
#include <QFile>
#include <QDir>
#include <QShortcut>
#include <QCompleter>
#include <QStringList>


PersistentConnection::PersistentConnection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersistentConnection)
{
    ui->setupUi(this);

    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(this->windowFlags() | Qt::WindowMaximizeButtonHint);

    suppressSlot = true;
    previousCommands.clear();



    QDEBUG();
    sendPacket.clear();
    QDEBUG() << ": refreshTimer Connection attempt " <<
                connect ( &refreshTimer , SIGNAL ( timeout() ) , this, SLOT ( refreshTimerTimeout (  ) ) )
             << connect ( this , SIGNAL ( rejected() ) , this, SLOT ( aboutToClose (  ) ) )
             << connect ( this , SIGNAL ( accepted() ) , this, SLOT ( aboutToClose (  ) ) )
             << connect ( this , SIGNAL ( dialogIsClosing() ) , this, SLOT ( aboutToClose (  ) ) );
    QDEBUG() << "Setup timer";
    refreshTimer.setInterval(200);
    refreshTimer.start();
    trafficList.clear();
    startTime = QDateTime::currentDateTime();
    wasConnected = false;
    stopTimer = false;

    ui->searchEdit->setText("");

    loadComboBox();

    useraw = true;

    ui->asciiLineEdit->setFocus();
    suppressSlot = false;

    QSettings settings(SETTINGSFILE, QSettings::IniFormat);
    bool asciiEditTranslateEBCDIC = settings.value("asciiEditTranslateEBCDICCheck", false).toBool();


    if(asciiEditTranslateEBCDIC) {
        QShortcut *shortcutEBCDIC = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_E), this);
        QDEBUG() << ": EBC connection attempt" << connect(shortcutEBCDIC, SIGNAL(activated()), this, SLOT(ebcdicTranslate()));

    }


}

void PersistentConnection::ebcdicTranslate() {

    QDEBUG() << "Translate ascii field";
        Packet ebcdicPkt;
        ebcdicPkt.init();
        QString oldascii = ui->asciiLineEdit->text();
        ebcdicPkt.hexString = Packet::ASCIITohex(oldascii);
        oldascii = ebcdicPkt.asciiString();

        QByteArray ebcdic = Packet::ASCIItoEBCDIC(ebcdicPkt.getByteArray());
        ebcdicPkt.hexString = Packet::byteArrayToHex(ebcdic);
        ui->asciiLineEdit->setText(ebcdicPkt.asciiString());
}

void PersistentConnection::loadComboBox() {

    QList<Packet> packetsSaved = Packet::fetchAllfromDB("");
    ui->packetComboBox->clear();
    Packet tempPacket;
    QString search = ui->searchEdit->text().trimmed().toLower();
    foreach(tempPacket, packetsSaved)
    {
        if(tempPacket.name.trimmed().toLower().contains(search)) {
            ui->packetComboBox->addItem(tempPacket.name);
        }

    }

}

void PersistentConnection::aboutToClose() {
    QDEBUG() << "Stopping timer";
    refreshTimer.stop();
    QDEBUG() << "checking thread null";
    if(thread == NULL) {
        QDEBUG() << "pointer is null";
    } else {
        QDEBUG() << "requesting stop";
        thread->closeRequest = true;
    }

    //cannot reliably call "wait" on a thread, so just exit.

}

void PersistentConnection::statusReceiver(QString message)
{
    //QDEBUGVAR(message);
    ui->topLabel->setText(message);

    if(message.startsWith("Connected")) {
        wasConnected = true;
    }

    if(message.toLower().startsWith("not connected")) {

        QDEBUG() << "Setting style sheet";
        ui->trafficViewEdit->setStyleSheet("QTextEdit { background-color: #EEEEEE }");
        ui->asciiSendButton->setEnabled(false);
        ui->asciiLineEdit->setEnabled(false);
        ui->packetComboBox->setEnabled(false);
        ui->appendCRcheck->setEnabled(false);
        ui->searchEdit->setEnabled(false);
        ui->packetComboBox->setEnabled(false);
        ui->LoadButton->setEnabled(false);

        ui->stopResendingButton->hide();
        stopTimer = true;
    }


}


PersistentConnection::~PersistentConnection()
{
    delete ui;
}



void PersistentConnection::connectThreadSignals()
{

    QDEBUG() << ": thread Connection attempt " <<
                connect ( this , SIGNAL ( persistentPacketSend(Packet) ) , thread, SLOT ( sendPersistant(Packet) ) )
             << connect ( this , SIGNAL ( closeConnection() ) , thread, SLOT ( closeConnection() ) )
             << connect ( thread , SIGNAL ( connectStatus(QString) ) , this, SLOT ( statusReceiver(QString) ) )
             << connect ( thread , SIGNAL ( packetSent(Packet)), this, SLOT(packetSentSlot(Packet)));

}

void PersistentConnection::initWithThread(TCPThread * thethread, quint16 portNum)
{


    setWindowTitle("TCP://You:" + QString::number(portNum));
    thread = thethread;
    QApplication::processEvents();
    connectThreadSignals();

    thread->start();

    ui->stopResendingButton->hide();

    QApplication::processEvents();
}


void PersistentConnection::init() {

    QString tcpOrSSL= "TCP";
    if(sendPacket.isSSL()) {
        tcpOrSSL = "SSL";
    }
    setWindowTitle(tcpOrSSL +"://"+sendPacket.toIP + ":" + QString::number(sendPacket.port));

    thread = new TCPThread(sendPacket, this);

    reSendPacket.clear();
    if(sendPacket.repeat > 0) {
        QDEBUG() << "This packet is repeating";
        reSendPacket = sendPacket;
    } else {

        ui->stopResendingButton->hide();
    }

    QApplication::processEvents();
    connectThreadSignals();

    thread->start();


    QApplication::processEvents();




    ui->stopResendingButton->setStyleSheet("QPushButton { color: black; } QPushButton::hover { color: #BC810C; } ");
    ui->stopResendingButton->setFlat(true);
    ui->stopResendingButton->setCursor(Qt::PointingHandCursor);
    ui->stopResendingButton->setIcon(QIcon(PSLOGO));

    connect(ui->stopResendingButton, &QPushButton::clicked, this, &PersistentConnection::cancelResends);

}


void PersistentConnection::cancelResends()
{
    QDEBUG();
    ui->stopResendingButton->hide();
    reSendPacket.clear();
}


void PersistentConnection::refreshTimerTimeout() {
//    QDEBUG();

    qint64 diff = startTime.msecsTo(QDateTime::currentDateTime());

    if(thread->isRunning() && !thread->closeRequest) {
        QString winTitle = windowTitle();
        if(winTitle.startsWith("TCP://") && thread->isEncrypted()) {
            winTitle.replace("TCP://", "SSL://");
            setWindowTitle(winTitle);
        }
    }



    qint64 hours = diff / (1000 * 60 * 60);
    qint64 diffRem = diff - hours * (1000 * 60 * 60);
    qint64 min = diffRem / (1000 * 60);
    diffRem = diffRem - min * (1000 * 60);
    qint64 sec = diffRem / (1000);

    QString datestamp = QString("%1:%2:%3")
            .arg(hours, 2, 10, QChar('0'))
            .arg(min, 2, 10, QChar('0'))
            .arg(sec, 2, 10, QChar('0'));

    if(wasConnected && !stopTimer) {

        ui->timeLabel->setText(datestamp);

        QDateTime now = QDateTime::currentDateTime();
        int repeatMS = (int) (reSendPacket.repeat * 1000 - 100);
        if(reSendPacket.timestamp.addMSecs(repeatMS) < now)
        {
            reSendPacket.timestamp = now;
            emit persistentPacketSend(reSendPacket);
        }

    }








//    QDEBUG() <<"Diff:" << diff;


}

void PersistentConnection::on_buttonBox_rejected()
{

    QDEBUG() << "Stopping timer";
    refreshTimer.stop();

}

void PersistentConnection::loadTrafficView() {


    QDEBUGVAR(trafficList.size());

    Packet loopPkt;

    QString html;
    html.clear();
    QTextStream out(&html);

    //        clipboard->setText(QString(savePacket.getByteArray()));

    if(useraw) {
        foreach(loopPkt, trafficList) {
                out << QString(loopPkt.getByteArray());
        }

        ui->trafficViewEdit->setPlainText(html);

    } else {



        out << "<html>" << "<b>" << trafficList.size() << " packets." << "</b><br>";

        int count = 0;
        foreach(loopPkt, trafficList) {
            QDEBUG() << "Packet Loop:" << count++ << loopPkt.asciiString();
            if(loopPkt.fromIP.toLower() == "you") {
                out << "<p style='color:blue'>";
            } else {
                out << "<p>";
            }
            out << loopPkt.asciiString().toHtmlEscaped();
            out << "</p>";
        }

        out << "</html>";
        ui->trafficViewEdit->setHtml(html);


    }



    QScrollBar *vScrollBar = ui->trafficViewEdit->verticalScrollBar();
    vScrollBar->triggerAction(QScrollBar::SliderToMaximum);
    QApplication::processEvents();

}

void PersistentConnection::packetSentSlot(Packet pkt) {

    QDEBUGVAR(pkt.hexString.size());
    trafficList.append(pkt);
    loadTrafficView();

}

void PersistentConnection::socketDisconnected()
{
    statusReceiver("not connected");
}

void PersistentConnection::on_asciiSendButton_clicked()
{
    QString ascii = ui->asciiLineEdit->text();
    if(ascii.isEmpty()) {
        return;
    }
    Packet asciiPacket;
    asciiPacket.clear();

    asciiPacket.tcpOrUdp = sendPacket.tcpOrUdp;
    asciiPacket.fromIP = "You";
    asciiPacket.toIP = sendPacket.toIP;
    asciiPacket.port = sendPacket.port;
    asciiPacket.hexString = Packet::ASCIITohex(ascii);

    previousCommands.append(ascii);
    previousCommands.removeDuplicates();

    QCompleter* completer = new QCompleter(previousCommands);

    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    ui->asciiLineEdit->setCompleter(completer);



    QDEBUGVAR(asciiPacket.hexString);
    if(ui->appendCRcheck->isChecked()) {
        asciiPacket.hexString.append(" 0d");
    }


    asciiPacket.receiveBeforeSend = false;

    emit persistentPacketSend(asciiPacket);
    ui->asciiLineEdit->setText("");

}

void PersistentConnection::on_packetComboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);


}

void PersistentConnection::on_searchEdit_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1);
    bool suppressSlotSave = suppressSlot;
    suppressSlot = true;
    loadComboBox();
    suppressSlot = suppressSlotSave;

}

void PersistentConnection::on_asciiCheck_clicked(bool checked)
{
    if(checked) {
        useraw = false;
    }
    loadTrafficView();


}

void PersistentConnection::on_rawCheck_clicked(bool checked)
{
    if(checked) {
        useraw = true;
    }
    loadTrafficView();


}

void PersistentConnection::on_LoadButton_clicked()
{
    Packet tempPacket;
    QString selectedName = ui->packetComboBox->currentText();
    QList<Packet> packetsSaved = Packet::fetchAllfromDB("");


    //QDEBUGVAR(selectedName);
    foreach(tempPacket, packetsSaved)
    {
        if(tempPacket.name == selectedName)
        {
            ui->asciiLineEdit->setText(tempPacket.asciiString());
            break;
        }

    }
}

void PersistentConnection::on_packetComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    if(!suppressSlot) {
        on_LoadButton_clicked();
    }

}

void PersistentConnection::on_clearButton_clicked()
{
    trafficList.clear();
    loadTrafficView();

}
