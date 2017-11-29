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
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QComboBox>

#include <QSerialPortInfo>
#include <QSerialPort>

#include <QToolButton>
#include <QToolBar>
#include <QDebug>
#include <QLabel>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "mainbox.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

    connect(&serial, SIGNAL(readyRead()), this, SLOT(port_read()));
    //connect(&serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(port_error(QSerialPort::SerialPortError)));
}
//--------------------------------------------------------------------------------
void MainBox::port_read(void)
{
    serial_data.append(serial.readAll());
}
//--------------------------------------------------------------------------------
void MainBox::port_error(QSerialPort::SerialPortError serial_error)
{
    switch(serial_error)
    {
    case QSerialPort::NoError:  break;
    case QSerialPort::DeviceNotFoundError:  emit error("Error: DeviceNotFoundError"); break;
    case QSerialPort::PermissionError:      emit error("Error: PermissionError"); break;
    case QSerialPort::OpenError:            emit error("Error: OpenError"); break;
    case QSerialPort::ParityError:          emit error("Error: ParityError"); break;
    case QSerialPort::FramingError:         emit error("Error: FramingError"); break;
    case QSerialPort::BreakConditionError:  emit error("Error: BreakConditionError"); break;
    case QSerialPort::WriteError:           emit error("Error: WriteError"); break;
    case QSerialPort::ReadError:            emit error("Error: ReadError"); break;
    case QSerialPort::ResourceError:        emit error("Error: ResourceError"); break;
    case QSerialPort::UnsupportedOperationError: emit error("Error: UnsupportedOperationError"); break;
    case QSerialPort::UnknownError:         emit error("Error: UnknownError"); break;
    case QSerialPort::TimeoutError:         emit error("Error: TimeoutError"); break;
    case QSerialPort::NotOpenError:         emit error("Error: NotOpenError"); break;
    default:
        emit error(QString("Unknown error %1").arg(serial_error));
        break;
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
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    if(mw == nullptr)
    {
        return;
    }

    QToolBar *toolBar = new QToolBar("testbar");
    toolBar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    btn_refresh_devices = add_button(toolBar,
                                     new QToolButton(this),
                                     qApp->style()->standardIcon(QStyle::SP_BrowserReload),
                                     tr("Обновить порты"),
                                     tr("Обновить порты"));

    toolBar->addWidget(new QLabel(tr("Порт:")));

    cb_ports = new QComboBox();
    refresh_devices();
    toolBar->addWidget(cb_ports);

    btn_open  = add_button(toolBar,
                           new QToolButton(this),
                           qApp->style()->standardIcon(QStyle::SP_DialogOpenButton),
                           tr("Открыть порт"),
                           tr("Открыть порт"));
    btn_close = add_button(toolBar,
                           new QToolButton(this),
                           qApp->style()->standardIcon(QStyle::SP_DialogCloseButton),
                           tr("Закрыть порт"),
                           tr("Закрыть порт"));
    btn_load_firmvare  = add_button(toolBar,
                                    new QToolButton(this),
                                    qApp->style()->standardIcon(QStyle::SP_DialogSaveButton),
                                    tr("Загрузить прошивку"),
                                    tr("Загрузить прошивку"));

    toolBar->setFixedSize(toolBar->sizeHint());

    connect(btn_refresh_devices,    SIGNAL(clicked()), this, SLOT(refresh_devices()));
    connect(btn_open,               SIGNAL(clicked()), this, SLOT(open()));
    connect(btn_close,              SIGNAL(clicked()), this, SLOT(close()));
    connect(btn_load_firmvare,      SIGNAL(clicked()), this, SLOT(load_firmvare()));
}
//--------------------------------------------------------------------------------
void MainBox::refresh_devices(void)
{
    cb_ports->clear();
    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
        cb_ports->addItem(port.portName());
    }
}
//--------------------------------------------------------------------------------
void MainBox::open(void)
{
    if(serial.isOpen()) serial.close();
    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
        if(cb_ports->currentText() == port.portName())
        {
            serial.setPort(port);
            int speed = 57600;
            bool ok = serial.setBaudRate(speed);
            if(ok)
            {
                if(serial.open(QIODevice::ReadWrite))
                {
                    emit info(QString(tr("Порт %1 успешно открыт на скорости %2"))
                              .arg(port.portName())
                              .arg(speed));
                    return;
                }
                else
                {
                    emit error(QString(tr("Порт %1 не может быть открыт на скорости %2"))
                              .arg(port.portName())
                              .arg(speed));
                    return;
                }
            }
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::close(void)
{
    if(serial.isOpen())
    {
        serial.close();
        emit info(tr("Порт закрыт"));
    }
    else
    {
        emit error(tr("Порт не был открыт!"));
    }
}
//--------------------------------------------------------------------------------
void MainBox::load_firmvare(void)
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
void MainBox::block_interface(bool state)
{
    QList<QDoubleSpinBox *> all_doublespinbox = topLevelWidget()->findChildren<QDoubleSpinBox *>();
    foreach(QDoubleSpinBox *obj, all_doublespinbox)
    {
        obj->setDisabled(state);
    }
    QList<QPushButton *> all_pushbutton = topLevelWidget()->findChildren<QPushButton *>();
    foreach(QPushButton *obj, all_pushbutton)
    {
        obj->setDisabled(state);
    }
    QList<QComboBox *> all_combobox = topLevelWidget()->findChildren<QComboBox *>();
    foreach(QComboBox *obj, all_combobox)
    {
        obj->setDisabled(state);
    }
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
