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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "ui_test_drawbuttons_mainbox.h"
#include "csvreader.hpp"
//--------------------------------------------------------------------------------
#include "CarefulMessageBox.hpp"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_drawbuttons_mainbox.hpp"

#include "mydisplay.hpp"
#include "mybutton.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
union U_BYTE
{
    unsigned char value;
    struct
    {
        unsigned bit0:1;
        unsigned bit1:1;
        unsigned bit2:1;
        unsigned bit3:1;
        unsigned bit4:1;
        unsigned bit5:1;
        unsigned bit6:1;
        unsigned bit7:1;
    } bits;
};
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

    draw_my_buttons();
}
//--------------------------------------------------------------------------------
void MainBox::draw_my_buttons(void)
{
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->setSpacing(0);

    MyDisplay *d[16];

    for(int n=0; n<16; n++)
    {
        d[n] = new MyDisplay(this);
        d[n]->display(n);
    }

    MyButton *b = new MyButton(100, 50, 3, tr("Привет"), this);
    connect(b, SIGNAL(clicked()), this, SLOT(test()));

    hbox->addWidget(b);
    hbox->addStretch();
    for(int n=0; n<16; n++)
        hbox->addWidget(d[n]);

    setLayout(hbox);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar(tr("testbar"));
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");
    
    connect(btn_test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
QStringList MainBox::qslGetPrintText(const QStringList qslStructText)
{
    QStringList qslPrintText=qslStructText;
    qslPrintText.replaceInStrings("$NUMBER_REFERENCE", "aaa");
    qslPrintText.replaceInStrings("$DATE_OUT", "bbb");
    qslPrintText.replaceInStrings("$DATE_SNJAT", "ccc");
    qslPrintText.replaceInStrings("$OSNOVANIE", "ddd");
    return qslPrintText;
}
//--------------------------------------------------------------------------------
void MainBox::set_item_param(QTreeWidgetItem *item,
                             int pos,
                             Qt::ItemFlags flags,
                             const QString &caption,
                             Qt::CheckState state)
{
    item->setFlags(item->flags() | flags);
    item->setText(pos, caption);
    item->setCheckState(pos, state);
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    emit debug(tr("тест"));

    CsvReader *csv = new CsvReader(0,"Склад_ПКИ.csv"); //
    if(csv->Open())
    {
        QList<QStringList> str = csv->CSVRead();
        foreach(QStringList sl, str)
        {
            emit debug(sl.at(1));
        }
        qDebug() << str.count();
    }
}
//--------------------------------------------------------------------------------
void MainBox::usb(void)
{
#if 0
    usb_init();
    //usb_set_debug();

    //usb_device dev;
    int busses = usb_find_busses();
    int devices = usb_find_devices();
    //usb_device_handle *handle = usb_open();
    qDebug() << busses << devices;

    struct usb_bus *bus;
    struct usb_device *dev;
    for(bus = usb_busses; bus; bus = bus->next)
    {
        for(dev = bus->devices; dev; dev = dev->next)
        {
            char string[256];
            usb_dev_handle *udev;
            udev = usb_open(dev);
            memset(string, 0, 256);
            usb_get_string_simple(udev, dev->descriptor.iManufacturer, string, 256);
            emit info(QString("Bus: %1, Device: %2 ID %3:%4, Продукт: %5")
                      .arg(bus->dirname)
                      .arg(dev->devnum)
                      .arg(dev->descriptor.idVendor, 0, 16)
                      .arg(dev->descriptor.idProduct, 0, 16)
                      .arg(string));
            //            char str[256];
            //            int ret = usb_get_string(udev, dev->descriptor.iManufacturer, dev->descriptor.iProduct, str, 256);
            //            if(ret > 0)
            //            {
            //                QByteArray ba;
            //                ba.clear();
            //                for(int n=0; n<256; n++)
            //                    if(isprint(str[n]))
            //                        ba.append(str[n]);
            //                qDebug() << ba.data();
            //            }
            usb_set_altinterface(udev, 0);
            int e = usb_claim_interface(udev, 0);
            qDebug() << "udev" << udev << "e" << e;
            //            switch(e)
            //            {
            //            case EBUSY:
            //                qDebug() << "Interface is not available to be claimed";
            //                break;
            //            case -ENOMEM:
            //                qDebug() << "Insufficient memory";
            //                break;
            //            default:
            //                qDebug() << "unknown code" << e;
            //                break;
            //            }

            usb_release_interface(udev, 0);
        }
    }
#endif
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
