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
#include <QTime>

#include <QAction>
#include <QMenu>

#include <QToolButton>
#include <QToolBar>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "mainbox.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
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

    serialBox5 = new SerialBox5(this, "RS485_5", "RS485");
    serialBox5->add_menu(2);

    ui->serial_layout->addWidget(serialBox5);

    connect(this,       SIGNAL(send(QByteArray)),   serialBox5, SLOT(input(QByteArray)));
    connect(serialBox5, SIGNAL(output(QByteArray)), this,       SLOT(read_data(QByteArray)));
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");
    
    connect(btn_test,   SIGNAL(clicked(bool)),  this,   SLOT(test()));
}
//--------------------------------------------------------------------------------
int MainBox::dev_set_reg(libusb_device_handle *device_handle, uint16_t wIndex, uint16_t 	wValue)
{
    uint8_t 	bmRequestType = LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_VENDOR;
    uint8_t 	bRequest      = XR_SET_REG;
    uint16_t 	wLength       = 0;
    uint16_t    *data         = 0;
    unsigned int timeout      = 5000;

    return libusb_control_transfer( device_handle, bmRequestType, bRequest, wValue, wIndex, (unsigned char*)data, wLength, timeout );
}
//--------------------------------------------------------------------------------
int MainBox::dev_get_reg(libusb_device_handle *device_handle, uint16_t wIndex, uint16_t *data)
{
    uint8_t 	bmRequestType = LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_VENDOR;
    uint8_t 	bRequest      = XR_GETN_REG;
    uint16_t 	wValue        = 0;
    uint16_t 	wLength       = 2;
    unsigned int timeout      = 5000;

    return libusb_control_transfer( device_handle, bmRequestType, bRequest, wValue, wIndex, (unsigned char*)data, wLength, timeout );
}
//--------------------------------------------------------------------------------
bool MainBox::test(void)
{
    emit info("test");

    int vendor = 0x1A86;

    libusb_context *ctx;
    if (libusb_init(&ctx)<0)
    {
        emit error("Error: Initializing libusb");
        return false;
    }

    libusb_device **device_list;
    ssize_t device_count = libusb_get_device_list(ctx, &device_list);

    for(ssize_t i = 0; i < device_count; ++i)
    {
        libusb_device *dev = device_list[i];

        if ( libusb_get_bus_number(dev) == 3)
        {
            libusb_device_descriptor device_descriptor;

            if ( libusb_get_device_descriptor(dev, &device_descriptor) >=0 )
            {
                if ( device_descriptor.idVendor == vendor )
                {
                    emit info(QString("FOUND %1:%2")
                               .arg(device_descriptor.idVendor, 4, 16, QChar('0'))
                               .arg(device_descriptor.idProduct, 4, 16, QChar('0')));

                    libusb_device_handle *device_handle;
                    if ( libusb_open(dev,&device_handle) >=0 )
                    {
                        libusb_free_device_list(device_list, 1);
                        int x0 = dev_set_reg(device_handle, CDC_ACM_FLOW_CONTROL,    0);
                        int x1 = dev_set_reg(device_handle, CDC_ACM_GPIO_MODE,      11);
                        int x2 = dev_set_reg(device_handle, CDC_ACM_GPIO_DIRECTION, 40);
                        int x3 = dev_set_reg(device_handle, CDC_ACM_GPIO_INT_MASK,   0);

                        emit info(QString("x0 = %1").arg(x0));
                        emit info(QString("x1 = %1").arg(x1));
                        emit info(QString("x2 = %1").arg(x2));
                        emit info(QString("x3 = %1").arg(x3));
                        libusb_close(device_handle);
                    }
                }
            }
        }
    }
    //libusb_device_handle *device_handle;

    emit info("The end!");
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray ba)
{
    emit info(ba.data());
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
