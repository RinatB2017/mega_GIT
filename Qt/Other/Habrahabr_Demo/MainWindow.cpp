#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // "Гасим" светодиод
    ui->led->turnOff();

    // Смена COM-порта
    connect(ui->portNameBox, SIGNAL(currentIndexChanged(int)), this, SLOT(checkCustomPortName(int)));
    // Смена скорости COM-порта
    connect(ui->baudRateBox, SIGNAL(currentIndexChanged(int)), this, SLOT(checkCustomBaudRatePolicy(int)));
    // Нажата кнопка подключения
    connect(ui->connectBtn, SIGNAL(clicked()), this, SIGNAL(processConnect()));
    // Отправка команды через консоль
    connect(ui->console, SIGNAL(onCommand(QString)), this, SIGNAL(onCommand(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getPortName() const
{
    return ui->portNameBox->currentText();
}

void MainWindow::setPortName(QString portName)
{
    ui->portNameBox->setCurrentText(portName);
}

void MainWindow::addPortName(QString portName)
{
    ui->portNameBox->addItem(portName);
}

int MainWindow::getBaudrate() const
{
    return ui->baudRateBox->currentText().toInt();
}

void MainWindow::setBaudrate(int baudrate)
{
    ui->baudRateBox->setCurrentText(QString::number(baudrate));
}

void MainWindow::addBaudrate(int baudrate)
{
    ui->baudRateBox->addItem(QString::number(baudrate));
}

void MainWindow::ledOn(bool on)
{
    ui->led->turnOn(on);
}

void MainWindow::setEnablePortNameBox(bool enable)
{
    ui->portNameBox->setEnabled(enable);
}

void MainWindow::setEnableBaudrateBox(bool enable)
{
    ui->baudRateBox->setEnabled(enable);
}

void MainWindow::setTitleButton(QString title)
{
    ui->connectBtn->setText(title);
}

void MainWindow::checkCustomBaudRatePolicy(int idx)
{
    Q_UNUSED(idx)
    emit processBaudratePortChanged(ui->baudRateBox->currentText().toInt());
}

void MainWindow::checkCustomPortName(int idx)
{
    Q_UNUSED(idx)
    emit processNameComPortChanged(ui->portNameBox->currentText());
}
