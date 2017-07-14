/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
**                                                                              **
**     This program is free software: you can redistribute it and/or modify     **
**     it under the terms of the GNU General Public License as published by     **
**     the Free Software Foundation, either version 3 of the License, or        **
**     (at your option) any later version.                                      **
**                                                                              **
**     This program is distributed in the hope that it will be useful,          **
**     but WITHOUT ANY WARRANTY; without even the implied warranty of           **
**     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **
**     GNU General Public License for more details.                             **
**                                                                              **
**     You should have received a copy of the GNU General Public License        **
**     along with this program.  If not, see http://www.gnu.org/licenses/.      **
**                                                                              **
**********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#include <QVBoxLayout>
#include <QMessageBox>
#include <QGroupBox>
#include <QTime>

#include <QAction>
#include <QMenu>

#include <QToolButton>
#include <QToolBar>
#include <QDebug>
//--------------------------------------------------------------------------------
#include <QModbusRtuSerialSlave>
#include <QModbusTcpServer>
#include <QRegularExpression>
#include <QStatusBar>
#include <QUrl>
//--------------------------------------------------------------------------------
#include "settingsdialog.h"
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
//--------------------------------------------------------------------------------
enum ModbusConnection {
    Serial,
    Tcp
};
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox),
    serialBox(0)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    if (modbusDevice)
        modbusDevice->disconnectDevice();
    delete modbusDevice;
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);
    connect_log();

    createTestBar();

    setupWidgetContainers();

    ui->connectType->setCurrentIndex(0);
    on_connectType_currentIndexChanged(0);

    m_settingsDialog = new SettingsDialog(this);
    initActions();
}
//--------------------------------------------------------------------------------
void MainBox::initActions(void)
{
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionExit->setEnabled(true);
    ui->actionOptions->setEnabled(true);

    connect(ui->actionConnect, &QAction::triggered,
            this, &MainBox::on_connectButton_clicked);
    connect(ui->actionDisconnect, &QAction::triggered,
            this, &MainBox::on_connectButton_clicked);

    connect(ui->actionExit, &QAction::triggered, this, &QMainWindow::close);
    connect(ui->actionOptions, &QAction::triggered, m_settingsDialog, &QDialog::show);
}
//--------------------------------------------------------------------------------
void MainBox::on_connectType_currentIndexChanged(int index)
{
    if (modbusDevice)
    {
        modbusDevice->disconnect();
        delete modbusDevice;
        modbusDevice = nullptr;
    }

    ModbusConnection type = static_cast<ModbusConnection> (index);
    if (type == Serial)
    {
        modbusDevice = new QModbusRtuSerialSlave(this);
    }
    else if (type == Tcp)
    {
        modbusDevice = new QModbusTcpServer(this);
        if (ui->portEdit->text().isEmpty())
            ui->portEdit->setText(QLatin1Literal("127.0.0.1:502"));
    }
    ui->listenOnlyBox->setEnabled(type == Serial);

    if (!modbusDevice)
    {
        MainWindow *mw = (MainWindow *)topLevelWidget();

        ui->connectButton->setDisabled(true);
        if (type == Serial)
        {
            if(mw) mw->statusBar()->showMessage(tr("Could not create Modbus slave."), 5000);
        }
        else
        {
            if(mw) mw->statusBar()->showMessage(tr("Could not create Modbus server."), 5000);
        }
    }
    else
    {
        QModbusDataUnitMap reg;
        reg.insert(QModbusDataUnit::Coils, { QModbusDataUnit::Coils, 0, 10 });
        reg.insert(QModbusDataUnit::DiscreteInputs, { QModbusDataUnit::DiscreteInputs, 0, 10 });
        reg.insert(QModbusDataUnit::InputRegisters, { QModbusDataUnit::InputRegisters, 0, 10 });
        reg.insert(QModbusDataUnit::HoldingRegisters, { QModbusDataUnit::HoldingRegisters, 0, 10 });

        modbusDevice->setMap(reg);

        connect(modbusDevice, &QModbusServer::dataWritten,
                this, &MainBox::updateWidgets);
        connect(modbusDevice, &QModbusServer::stateChanged,
                this, &MainBox::onStateChanged);
        connect(modbusDevice, &QModbusServer::errorOccurred,
                this, &MainBox::handleDeviceError);

        connect(ui->listenOnlyBox, &QCheckBox::toggled, this, [this](bool toggled)
        {
            if (modbusDevice)
                modbusDevice->setValue(QModbusServer::ListenOnlyMode, toggled);
        });
        emit ui->listenOnlyBox->toggled(ui->listenOnlyBox->isChecked());
        connect(ui->setBusyBox, &QCheckBox::toggled, this, [this](bool toggled)
        {
            if (modbusDevice)
                modbusDevice->setValue(QModbusServer::DeviceBusy, toggled ? 0xffff : 0x0000);
        });
        emit ui->setBusyBox->toggled(ui->setBusyBox->isChecked());

        setupDeviceData();
    }
}
//--------------------------------------------------------------------------------
void MainBox::handleDeviceError(QModbusDevice::Error newError)
{
    if (newError == QModbusDevice::NoError || !modbusDevice)
        return;

    MainWindow *mw = (MainWindow *)topLevelWidget();
    if(mw) mw->statusBar()->showMessage(modbusDevice->errorString(), 5000);
}
//--------------------------------------------------------------------------------
void MainBox::on_connectButton_clicked()
{
    bool intendToConnect = (modbusDevice->state() == QModbusDevice::UnconnectedState);

    MainWindow *mw = (MainWindow *)topLevelWidget();
    if(mw) mw->statusBar()->clearMessage();

    if (intendToConnect)
    {
        if (static_cast<ModbusConnection> (ui->connectType->currentIndex()) == Serial)
        {
            modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
                                                 ui->portEdit->text());
            modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter,
                                                 m_settingsDialog->settings().parity);
            modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
                                                 m_settingsDialog->settings().baud);
            modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
                                                 m_settingsDialog->settings().dataBits);
            modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
                                                 m_settingsDialog->settings().stopBits);
        }
        else
        {
            const QUrl url = QUrl::fromUserInput(ui->portEdit->text());
            modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, url.port());
            modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, url.host());
        }
        modbusDevice->setServerAddress(ui->serverEdit->text().toInt());
        if (!modbusDevice->connectDevice())
        {
            MainWindow *mw = (MainWindow *)topLevelWidget();
            if(mw) mw->statusBar()->showMessage(tr("Connect failed: ") + modbusDevice->errorString(), 5000);
        }
        else
        {
            ui->actionConnect->setEnabled(false);
            ui->actionDisconnect->setEnabled(true);
        }
    }
    else
    {
        modbusDevice->disconnectDevice();
        ui->actionConnect->setEnabled(true);
        ui->actionDisconnect->setEnabled(false);
    }
}
//--------------------------------------------------------------------------------
void MainBox::onStateChanged(int state)
{
    bool connected = (state != QModbusDevice::UnconnectedState);
    ui->actionConnect->setEnabled(!connected);
    ui->actionDisconnect->setEnabled(connected);

    if (state == QModbusDevice::UnconnectedState)
        ui->connectButton->setText(tr("Connect"));
    else if (state == QModbusDevice::ConnectedState)
        ui->connectButton->setText(tr("Disconnect"));
}
//--------------------------------------------------------------------------------
void MainBox::coilChanged(int id)
{
    QAbstractButton *button = coilButtons.button(id);
    bitChanged(id, QModbusDataUnit::Coils, button->isChecked());
}
//--------------------------------------------------------------------------------
void MainBox::discreteInputChanged(int id)
{
    QAbstractButton *button = discreteButtons.button(id);
    bitChanged(id, QModbusDataUnit::DiscreteInputs, button->isChecked());
}
//--------------------------------------------------------------------------------
void MainBox::bitChanged(int id, QModbusDataUnit::RegisterType table, bool value)
{
    if (!modbusDevice)
        return;

    MainWindow *mw = (MainWindow *)topLevelWidget();
    if (!modbusDevice->setData(table, id, value))
    {
        if(mw) mw->statusBar()->showMessage(tr("Could not set data: ") + modbusDevice->errorString(), 5000);
    }
}
//--------------------------------------------------------------------------------
void MainBox::setRegister(const QString &value)
{
    if (!modbusDevice)
        return;

    MainWindow *mw = (MainWindow *)topLevelWidget();
    const QString objectName = QObject::sender()->objectName();
    if (registers.contains(objectName))
    {
        bool ok = true;
        const int id = QObject::sender()->property("ID").toInt();
        if (objectName.startsWith(QStringLiteral("inReg")))
            ok = modbusDevice->setData(QModbusDataUnit::InputRegisters, id, value.toInt(&ok, 16));
        else if (objectName.startsWith(QStringLiteral("holdReg")))
            ok = modbusDevice->setData(QModbusDataUnit::HoldingRegisters, id, value.toInt(&ok, 16));

        if (!ok)
        {
            if(mw) mw->statusBar()->showMessage(tr("Could not set register: ") + modbusDevice->errorString(), 5000);
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::updateWidgets(QModbusDataUnit::RegisterType table, int address, int size)
{
    for (int i = 0; i < size; ++i)
    {
        quint16 value;
        QString text;
        switch (table)
        {
        case QModbusDataUnit::Coils:
            modbusDevice->data(QModbusDataUnit::Coils, address + i, &value);
            coilButtons.button(address + i)->setChecked(value);
            break;

        case QModbusDataUnit::HoldingRegisters:
            modbusDevice->data(QModbusDataUnit::HoldingRegisters, address + i, &value);
            registers.value(QStringLiteral("holdReg_%1").arg(address + i))->setText(text.setNum(value, 16));
            break;

        default:
            break;
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::setupDeviceData()
{
    if (!modbusDevice)
        return;

    for (int i = 0; i < coilButtons.buttons().count(); ++i)
        modbusDevice->setData(QModbusDataUnit::Coils, i, coilButtons.button(i)->isChecked());

    for (int i = 0; i < discreteButtons.buttons().count(); ++i)
    {
        modbusDevice->setData(QModbusDataUnit::DiscreteInputs, i,
                              discreteButtons.button(i)->isChecked());
    }

    bool ok;
    foreach (QLineEdit *widget, registers)
    {
        if (widget->objectName().startsWith(QStringLiteral("inReg")))
        {
            modbusDevice->setData(QModbusDataUnit::InputRegisters, widget->property("ID").toInt(),
                                  widget->text().toInt(&ok, 16));
        }
        else if (widget->objectName().startsWith(QStringLiteral("holdReg")))
        {
            modbusDevice->setData(QModbusDataUnit::HoldingRegisters, widget->property("ID").toInt(),
                                  widget->text().toInt(&ok, 16));
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::setupWidgetContainers()
{
    coilButtons.setExclusive(false);
    discreteButtons.setExclusive(false);

    QRegularExpression regexp(QStringLiteral("coils_(?<ID>\\d+)"));
    const QList<QCheckBox *> coils = findChildren<QCheckBox *>(regexp);
    foreach (QCheckBox *cbx, coils)
        coilButtons.addButton(cbx, regexp.match(cbx->objectName()).captured("ID").toInt());
    connect(&coilButtons, SIGNAL(buttonClicked(int)), this, SLOT(coilChanged(int)));

    regexp.setPattern(QStringLiteral("disc_(?<ID>\\d+)"));
    const QList<QCheckBox *> discs = findChildren<QCheckBox *>(regexp);
    foreach (QCheckBox *cbx, discs)
        discreteButtons.addButton(cbx, regexp.match(cbx->objectName()).captured("ID").toInt());
    connect(&discreteButtons, SIGNAL(buttonClicked(int)), this, SLOT(discreteInputChanged(int)));

    regexp.setPattern(QLatin1String("(in|hold)Reg_(?<ID>\\d+)"));
    const QList<QLineEdit *> qle = findChildren<QLineEdit *>(regexp);
    foreach (QLineEdit *lineEdit, qle)
    {
        registers.insert(lineEdit->objectName(), lineEdit);
        lineEdit->setProperty("ID", regexp.match(lineEdit->objectName()).captured("ID").toInt());
        lineEdit->setValidator(new QRegExpValidator(QRegExp(QStringLiteral("[0-9a-f]{0,4}"),
                                                            Qt::CaseInsensitive), this));
        connect(lineEdit, &QLineEdit::textChanged, this, &MainBox::setRegister);
    }
}
//--------------------------------------------------------------------------------
QToolButton *MainBox::add_button(QToolBar *tool_bar,
                                 QToolButton *tool_button,
                                 QIcon icon,
                                 const QString &text,
                                 const QString &tool_tip)
{
    if(!tool_bar) return NULL;
    if(!tool_button) return NULL;

    tool_button->setIcon(icon);
    tool_button->setText(text);
    tool_button->setToolTip(tool_tip);
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    QToolBar *toolBar = new QToolBar(tr("testbar"));

    MainWindow *mw = (MainWindow *)parentWidget();
    if(!mw) return;

    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    QToolButton *btn_test = add_button(toolBar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");

    connect(btn_test,       SIGNAL(clicked()),      this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
