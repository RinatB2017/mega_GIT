#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qdebug.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tracker = NULL;

    // Connect signals and slots
    connect(ui->pbConnect,  SIGNAL(clicked(bool)),      this,   SLOT(onConnectClicked()));
    connect(this,           SIGNAL(consoleChanged()),   this,   SLOT(onConsoleChanged()));
    connect(ui->pbSend,     SIGNAL(clicked(bool)),      this,   SLOT(onSendClicked()));
    connect(ui->leCmd,      SIGNAL(returnPressed()),    this,   SLOT(onSendClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onDataRecv(const char *data, int size, void *bundle)
{
    char buf[size + 1];
    strncpy(buf, data, size);
    buf[size] = 0;

    MainWindow *window = (MainWindow*) bundle;
    window->consoleText.append(QString(buf));
    emit window->consoleChanged();
}

void MainWindow::onConnectClicked()
{
    int r;

    ui->pbConnect->setEnabled(false); // Temporary disable button

    if (!tracker)
    {
        if (ui->leHost->text().length() == 0 || ui->lePort->text().length() == 0)
        {
            ui->pbConnect->setEnabled(true);
            return;
        }

        qDebug() << "Connecting";
        // Create new tracker for new connection
        tracker = new qtelnet;
        // Add (optional) callback for receiving data from server
        tracker->set_data_recv_callback(&(MainWindow::onDataRecv));
        tracker->set_data_recv_bundle((void*) this);

        r = qtelnet::telnet_connect(*tracker,
                                    ui->leHost->text().toStdString().c_str(),
                                    ui->lePort->text().toStdString().c_str());
        if (r == 0)
        {
            // Connection success
            ui->pbConnect->setText("Disconnect");
            ui->leHost->setEnabled(false);
            ui->lePort->setEnabled(false);
            ui->leUsername->setEnabled(false);
            ui->lePassword->setEnabled(false);
        }
        else
        {
            // When fail, clean up
            qtelnet::telnet_disconnect(*tracker);
            delete tracker;
            tracker = NULL;
        }
    }
    else
    {
        qDebug() << "Disconnect";
        qtelnet::telnet_disconnect(*tracker);
        delete tracker;
        tracker = NULL;
        ui->pbConnect->setText("Connect");
        ui->leHost->setEnabled(true);
        ui->lePort->setEnabled(true);
        ui->leUsername->setEnabled(true);
        ui->lePassword->setEnabled(true);
    }

    ui->pbConnect->setEnabled(true);
}

void MainWindow::onConsoleChanged()
{
    ui->lbConsole->setText(consoleText);

    // Autologin
    static QRegExp regxInputUsername(".* login: $");
    static QRegExp regxInputPassword(".* Password: $");

    if (ui->leUsername->text().length() > 0)
    {
        if (regxInputUsername.exactMatch(ui->lbConsole->text()))
        {
            qDebug() << "Sending username";
            qtelnet::send_text(*tracker,
                               ui->leUsername->text().toStdString().c_str(),
                               ui->leUsername->text().length());
        }
        if (regxInputPassword.exactMatch(ui->lbConsole->text()))
        {
            qDebug() << "Sending password";
            qtelnet::send_text(*tracker,
                               ui->lePassword->text().toStdString().c_str(),
                               ui->lePassword->text().length());
        }
    }
}

void MainWindow::onSendClicked()
{
    if (ui->leCmd->text().length() == 0) return;

    if (tracker)
    {
        qtelnet::send_text(*tracker,
                           ui->leCmd->text().toStdString().c_str(),
                           ui->leCmd->text().length());
    }
    //ui->leCmd->clear();
}
