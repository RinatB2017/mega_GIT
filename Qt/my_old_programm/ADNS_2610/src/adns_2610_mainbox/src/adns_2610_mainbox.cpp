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
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "ui_adns_2610_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "screen.hpp"
#include "adns_2610_mainbox.hpp"
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

#ifdef QT_DEBUG
    createTestBar();
#endif

    serialBox = new SerialBox5(this, "RS232", "RS232");
    serialBox->add_menu(2);
    serialBox->setFixedWidth(serialBox->sizeHint().width());

    data_rs232.clear();

    screen = new Screen(this);

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(serialBox);
    hbox->addWidget(screen);
    setLayout(hbox);

    connect(this, SIGNAL(send(QByteArray)), serialBox, SLOT(input(QByteArray)));
    connect(serialBox, SIGNAL(output(QByteArray)), this, SLOT(read_data(QByteArray)));
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *test = add_button(testbar,
                                   new QToolButton(this),
                                   qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                   "choice_test",
                                   "choice_test");
    connect(test, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    QByteArray ba;
    ba.clear();

    QString temp;
    temp.clear();
    temp.append(":");
    uint8_t i = 0;
    for(int n=0; n<(18*18); n++)
    {
        temp.append(convert_data_to_ascii(i));
        ba.append(i);
        i++;
    }
    temp.append((char)0x0D);

    QByteArray t;
    t.clear();
    t.append(temp.mid(1, temp.length()-1).toLatin1());

    QByteArray data;
    data.clear();
    data.append(QByteArray::fromHex(t));

    screen->draw(ba);
}
//--------------------------------------------------------------------------------
QString MainBox::convert_data_to_ascii(uint8_t data)
{
    uint8_t hi = (data >> 4) & 0x0F;
    uint8_t lo = (data & 0x0F);

    QString hi_str;
    switch(hi)
    {
    case 0x00: hi_str="0"; break;
    case 0x01: hi_str="1"; break;
    case 0x02: hi_str="2"; break;
    case 0x03: hi_str="3"; break;
    case 0x04: hi_str="4"; break;
    case 0x05: hi_str="5"; break;
    case 0x06: hi_str="6"; break;
    case 0x07: hi_str="7"; break;
    case 0x08: hi_str="8"; break;
    case 0x09: hi_str="9"; break;
    case 0x0A: hi_str="A"; break;
    case 0x0B: hi_str="B"; break;
    case 0x0C: hi_str="C"; break;
    case 0x0D: hi_str="D"; break;
    case 0x0E: hi_str="E"; break;
    case 0x0F: hi_str="F"; break;
    default: break;
    }

    QString lo_str;
    switch(lo)
    {
    case 0x00: lo_str="0"; break;
    case 0x01: lo_str="1"; break;
    case 0x02: lo_str="2"; break;
    case 0x03: lo_str="3"; break;
    case 0x04: lo_str="4"; break;
    case 0x05: lo_str="5"; break;
    case 0x06: lo_str="6"; break;
    case 0x07: lo_str="7"; break;
    case 0x08: lo_str="8"; break;
    case 0x09: lo_str="9"; break;
    case 0x0A: lo_str="A"; break;
    case 0x0B: lo_str="B"; break;
    case 0x0C: lo_str="C"; break;
    case 0x0D: lo_str="D"; break;
    case 0x0E: lo_str="E"; break;
    case 0x0F: lo_str="F"; break;
    default: break;
    }

    return (hi_str+lo_str);
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray ba)
{
    QByteArray t;
    QByteArray data;

    //emit info("read_data");
    for(int n=0; n<ba.length(); n++)
    {
        switch(ba.at(n))
        {
        case ':':
            data_rs232.clear();
            break;

        case 0x0D:
            t.clear();
            t.append(data_rs232.mid(1, data_rs232.length()-1));

            data.clear();
            data.append(QByteArray::fromHex(t));

            screen->draw(data);
            data_rs232.clear();
            break;

        default:
            data_rs232.append(ba.at(n));
            break;
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
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
