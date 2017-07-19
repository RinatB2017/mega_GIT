/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#include <QGridLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTime>

#include <QAction>
#include <QMenu>

#include <QPushButton>
#include <QToolButton>
#include <QToolBar>
#include <QSpinBox>
#include <QTimer>
#include <QLabel>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "ui_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "serialbox.hpp"
#include "mainbox.hpp"
#include "defines.hpp"
#include "sleeper.h"
//--------------------------------------------------------------------------------
#include "test_glass.hpp"
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
    serialBox->deleteLater();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();
    createSerialBox();
    createGridBox();

    init_widgets();

#if 1
    QPixmap pix;
    pix.load(":/pic/imgpsh_fullsize.jpe");
    ui->lbl_pic->setPixmap(pix);
    ui->lbl_pic->setFixedSize(pix.size());
#else
    QPixmap pix;
    pix.load(":/pic/lustre.jpg");
    qDebug() << pix.width() << pix.height();
    int w = 500.0f * 2448.0f / 3264.0f;
    int h = 500; //ui->lbl_pic->height();
    qDebug() << w << h;
    QPixmap new_pix = pix.scaled(w, h, Qt::KeepAspectRatio);
    ui->lbl_pic->setPixmap(new_pix);
    ui->lbl_pic->setFixedSize(new_pix.size());
#endif
    Test_Glass *glass = new Test_Glass(this);
    glass->install(ui->lbl_pic);
}
//--------------------------------------------------------------------------------
void MainBox::init_widgets(void)
{

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
    toolBar->setObjectName("testbar");

    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());

    if(!mw) return;

    mw->addToolBar(Qt::TopToolBarArea, toolBar);

    sb_min = new QSpinBox(this);
    sb_min->setObjectName("sb_min");
    sb_min->setMinimum(0);
    sb_min->setMaximum(100);
    sb_min->setValue(0);

    sb_max = new QSpinBox(this);
    sb_max->setObjectName("sb_max");
    sb_max->setMinimum(0);
    sb_max->setMaximum(100);
    sb_max->setValue(100);

    toolBar->addWidget(new QLabel("min"));
    toolBar->addWidget(sb_min);
    toolBar->addWidget(new QLabel("max"));
    toolBar->addWidget(sb_max);

    QToolButton *btn_on = add_button(toolBar,
                                     new QToolButton(this),
                                     QIcon(),   //qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                     "1",
                                     "1");
    QToolButton *btn_off = add_button(toolBar,
                                      new QToolButton(this),
                                      QIcon(),  //qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                      "0",
                                      "0");

    QToolButton *btn_test = add_button(toolBar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "0",
                                       "0");

    connect(btn_on,     SIGNAL(clicked(bool)), this, SLOT(on()));
    connect(btn_off,    SIGNAL(clicked(bool)), this, SLOT(off()));

    connect(btn_test,   SIGNAL(clicked(bool)), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
void MainBox::createGridBox(void)
{
    QGridLayout *grid = new QGridLayout(this);
    grid->setMargin(0);
    grid->setSpacing(0);
    for(int y=0; y<MAX_SCREEN_Y; y++)
    {
        for(int x=0; x<MAX_SCREEN_X; x++)
        {
            btn[x][y] = new QToolButton(this);
            btn[x][y]->setCheckable(true);
            btn[x][y]->setText(QString("%1:%2").arg(x).arg(y));
            btn[x][y]->setProperty("property_x", x);
            btn[x][y]->setProperty("property_y", y);
            connect(btn[x][y], SIGNAL(clicked(bool)),   this,   SLOT(btn_click(bool)));

            grid->addWidget(btn[x][y], x, y);
        }
    }

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addLayout(grid);
    hbox->addStretch(1);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(hbox);
    vbox->addStretch(1);

    ui->frame->setLayout(vbox);
}
//--------------------------------------------------------------------------------
void MainBox::btn_click(bool state)
{
    Q_UNUSED(state)

    QToolButton *btn = (QToolButton *)sender();
    if(!btn) return;

    int x = btn->property("property_x").toInt();
    int y = btn->property("property_y").toInt();

    F_01 packet;

    packet.body.header.addr = 0;
    packet.body.header.cmd = CMD_0x01;
    packet.body.header.len = sizeof(packet.body.data_t);

    uint8_t max = sb_max->value();
    uint8_t min = sb_min->value();

    buf_leds[x][y] = btn->isChecked() ? max : min;

    for(int y=0; y<MAX_SCREEN_Y; y++)
    {
        for(int x=0; x<MAX_SCREEN_X; x++)
        {
            packet.body.data_t.leds[y][x] = buf_leds[x][y];
        }
    }

    QByteArray ba;
    ba.clear();
    ba.append((char *)&packet.buf, sizeof(packet));
    emit info(ba.toHex());

    QString send_packet = QString(":%1\n")
            .arg(ba.toHex().data());

    QByteArray o_ba;
    o_ba.clear();
    o_ba.append(send_packet);
    emit send(o_ba);
}
//--------------------------------------------------------------------------------
void MainBox::createSerialBox(void)
{
    serialBox = new SerialBox(this, "RS232");
    serialBox->add_menu(2);

    ui->serial_layout->addWidget(serialBox);
    ui->serial_layout->addStretch();

    connect(this,       SIGNAL(send(QByteArray)),   serialBox,  SLOT(input(QByteArray)));
    connect(serialBox,  SIGNAL(output(QByteArray)), this,       SLOT(read_data(QByteArray)));
}
//--------------------------------------------------------------------------------
void MainBox::on(void)
{
    emit info("ON");

    F_01 packet;

    packet.body.header.addr = 0;
    packet.body.header.cmd = CMD_0x01;
    packet.body.header.len = sizeof(packet.body.data_t);

    for(int y=0; y<MAX_SCREEN_Y; y++)
    {
        for(int x=0; x<MAX_SCREEN_X; x++)
        {
            packet.body.data_t.leds[y][x] = sb_max->value();
        }
    }

    QByteArray ba;
    ba.clear();
    ba.append((char *)&packet.buf, sizeof(packet));

    QString send_packet = QString(":%1\n")
            .arg(ba.toHex().data())
            .toUpper();
    emit info(send_packet);

    QByteArray o_ba;
    o_ba.clear();
    o_ba.append(send_packet);
    emit send(o_ba);

}
//--------------------------------------------------------------------------------
void MainBox::off(void)
{
    emit info("OFF");

    F_01 packet;

    packet.body.header.addr = 0;
    packet.body.header.cmd = CMD_0x01;
    packet.body.header.len = sizeof(packet.body.data_t);

    for(int y=0; y<MAX_SCREEN_Y; y++)
    {
        for(int x=0; x<MAX_SCREEN_X; x++)
        {
            packet.body.data_t.leds[y][x] = sb_min->value();
        }
    }

    QByteArray ba;
    ba.clear();
    ba.append((char *)&packet.buf, sizeof(packet));

    QString send_packet = QString(":%1\n")
            .arg(ba.toHex().data())
            .toUpper();
    emit info(send_packet);

    QByteArray o_ba;
    o_ba.clear();
    o_ba.append(send_packet);
    emit send(o_ba);
}
//--------------------------------------------------------------------------------
void MainBox::click(void)
{
    QList<QPushButton *> all_obj = topLevelWidget()->findChildren<QPushButton *>();
    foreach(QPushButton *obj, all_obj)
    {
        if(obj->property("button").toBool())
        {
            if(obj->isChecked())
            {
                emit info(obj->objectName());
            }
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::test(void)
{
    F_01 packet;

    packet.body.header.addr = 0;
    packet.body.header.cmd = CMD_0x01;
    packet.body.header.len = sizeof(packet.body.data_t);

    buf_leds[4][1] = 5;
    buf_leds[5][1] = 5;

    buf_leds[3][2] = 5;
    buf_leds[4][5] = 5;

    buf_leds[5][2] = 5;
    buf_leds[1][3] = 5;

    buf_leds[5][3] = 5;
    buf_leds[3][4] = 5;

    buf_leds[0][5] = 5;
    buf_leds[1][0] = 5;

    buf_leds[4][0] = 5;
    buf_leds[1][1] = 5;

    for(int y=0; y<MAX_SCREEN_Y; y++)
    {
        for(int x=0; x<MAX_SCREEN_X; x++)
        {
            packet.body.data_t.leds[y][x] = buf_leds[x][y];
        }
    }

    QByteArray ba;
    ba.clear();
    ba.append((char *)&packet.buf, sizeof(packet));
    emit info(ba.toHex());

    QString send_packet = QString(":%1\n")
            .arg(ba.toHex().data());

    QByteArray o_ba;
    o_ba.clear();
    o_ba.append(send_packet);
    emit send(o_ba);
}
//--------------------------------------------------------------------------------
void MainBox::test2(void)
{
    int btn = QMessageBox::question(this,
                                    "Тест",
                                    "Вы хотите запустить тест?",
                                    QMessageBox::Yes | QMessageBox::No);
    if(btn != QMessageBox::Yes)
    {
        return;
    }

    block_this_button(true);
#if 0
    emit info("TEST2");
#else
    QByteArray ba;

    char val = 0;
    int cnt = 0;
    int max_cnt = 5;
    while(cnt<max_cnt)
    {
        cnt++;
        emit info(QString("cnt %1 max_cnt %2")
                  .arg(cnt)
                  .arg(max_cnt));
        for(val=0; val<=100; val++)
        {
            QCoreApplication::processEvents();
            ba.clear();
            ba.append((char)0);
            ba.append((char)0);
            ba.append((char)val);

            QString send_packet = QString(":%1\n")
                    .arg(ba.toHex().data());

            //emit info(QString("send: %1").arg(send_packet));

            QByteArray o_ba;
            o_ba.clear();
            o_ba.append(send_packet);
            emit send(o_ba);

            Sleeper::msleep(100);
        }
    }
#endif
    block_this_button(false);
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray ba)
{
    emit debug("read_data");
    data_rs232.append(ba);
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
uint8_t MainBox::convert_ascii_to_value(char hi, char lo)
{
    uint8_t b_hi = 0;
    uint8_t b_lo = 0;

    //---
    switch(hi)
    {
    case '0':  b_hi = 0x0;  break;
    case '1':  b_hi = 0x1;  break;
    case '2':  b_hi = 0x2;  break;
    case '3':  b_hi = 0x3;  break;
    case '4':  b_hi = 0x4;  break;
    case '5':  b_hi = 0x5;  break;
    case '6':  b_hi = 0x6;  break;
    case '7':  b_hi = 0x7;  break;
    case '8':  b_hi = 0x8;  break;
    case '9':  b_hi = 0x9;  break;
    case 'A':  b_hi = 0xA;  break;
    case 'B':  b_hi = 0xB;  break;
    case 'C':  b_hi = 0xC;  break;
    case 'D':  b_hi = 0xD;  break;
    case 'E':  b_hi = 0xE;  break;
    case 'F':  b_hi = 0xF;  break;
    default:
        break;
    }
    //---
    switch(lo)
    {
    case '0':  b_lo = 0x0;  break;
    case '1':  b_lo = 0x1;  break;
    case '2':  b_lo = 0x2;  break;
    case '3':  b_lo = 0x3;  break;
    case '4':  b_lo = 0x4;  break;
    case '5':  b_lo = 0x5;  break;
    case '6':  b_lo = 0x6;  break;
    case '7':  b_lo = 0x7;  break;
    case '8':  b_lo = 0x8;  break;
    case '9':  b_lo = 0x9;  break;
    case 'A':  b_lo = 0xA;  break;
    case 'B':  b_lo = 0xB;  break;
    case 'C':  b_lo = 0xC;  break;
    case 'D':  b_lo = 0xD;  break;
    case 'E':  b_lo = 0xE;  break;
    case 'F':  b_lo = 0xF;  break;
    default:
        break;
    }
    //---
    uint8_t r_byte = (b_hi << 4) | b_lo;
    return r_byte;
}
//--------------------------------------------------------------------------------
void MainBox::block_this_button(bool state)
{
    QObject *btn = (QObject *)sender();
    if(!btn) return;

    if (QPushButton *pb=qobject_cast<QPushButton *>(btn))
    {
        pb->setDisabled(state);
    }
    if (QToolButton *tb=qobject_cast<QToolButton *>(btn))
    {
        tb->setDisabled(state);
    }
}
//--------------------------------------------------------------------------------
void MainBox::block_interface(bool state)
{
    QList<QPushButton *> all_pushbutton = topLevelWidget()->findChildren<QPushButton *>();
    foreach(QPushButton *obj, all_pushbutton)
    {
        obj->setDisabled(state);
    }
}
//--------------------------------------------------------------------------------
void MainBox::block_widget(const QString name, bool state)
{
    if(name.isEmpty())
    {
        return;
    }
    QList<QWidget *> all_obj = topLevelWidget()->findChildren<QWidget *>();
    foreach(QWidget *obj, all_obj)
    {
        if(obj->objectName() == name)
        {
            obj->setDisabled(state);
        }
    }
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
