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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "ui_test_newmoonlight_firmware_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "test_newmoonlight_firmware_mainbox.hpp"
#include "defines.hpp"
#include "sleeper.h"
//--------------------------------------------------------------------------------
#include "button.hpp"
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

    QPixmap pix;
    pix.load(":/pic/imgpsh_fullsize.jpe");
    ui->lbl_pic->setPixmap(pix);
    ui->lbl_pic->setFixedSize(pix.size());

    connect(ui->sl_cold,    SIGNAL(valueChanged(int)),  ui->sb_cold,    SLOT(setValue(int)));
    connect(ui->sl_hot,     SIGNAL(valueChanged(int)),  ui->sb_hot,     SLOT(setValue(int)));

    connect(ui->sl_cold,    SIGNAL(valueChanged(int)),  this,   SLOT(send_cold_pic_question(int)));
    connect(ui->sl_hot,    SIGNAL(valueChanged(int)),  this,   SLOT(send_hot_pic_question(int)));

    connect(sl_value,   SIGNAL(valueChanged(int)),  this,   SLOT(send_grid_question(int)));

    init_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::init_widgets(void)
{
    //---
    int index = 0;
    center_x = ui->lbl_pic->width() / 2.0f;
    center_y = ui->lbl_pic->height() / 2.0f;
    center_r = ui->lbl_pic->width() / 14.0f;
    led_r = ui->lbl_pic->width() / 17.0f;
    min_r = center_r + led_r + 10.0f;
    max_r = ui->lbl_pic->width() / 2.0f - 20.0f;
    min_angle = 270.0f; //-30.0f;
    max_angle = -90.0f; //330.0f;
    inc_r = (int)((max_r - min_r) / 2.4f);
    qreal angle = min_angle;
    int i = 0;
    while(angle > max_angle)
    {
        for(int n=0; n<3; n++)
        {
            calc_line(center_x,
                      center_y,
                      angle,
                      inc_r * (n + 1),
                      &temp_x,
                      &temp_y);
            Button *btn = new Button(led_r*2,
                                     led_r*2,
                                     i,
                                     ui->lbl_pic);
            btn->setCheckable(true);
            pic_buttons.append(btn);
            btn->set_color(Qt::red, Qt::black);
            btn->move(temp_x-led_r,
                      temp_y-led_r);
            // connect(btn,    SIGNAL(toggled(bool)),  this,   SLOT(btn_click_adv(bool)));
            index++;
            i++;
        }
        angle -= 60.0;
    }

    set_property(0,   0x2112);
    set_property(1,   0x4151);
    set_property(2,   0x3102);

    set_property(3,   0x2255);
    set_property(4,   0x3245);
    set_property(5,   0x2535);

    set_property(6,   0x4223);
    set_property(7,   0x5213);
    set_property(8,   0x0333);

    set_property(9,   0x4314);
    set_property(10,  0x5334);
    set_property(11,  0x0424);

    set_property(12,  0x5415);
    set_property(13,  0x0510);
    set_property(14,  0x4400);

    set_property(15,  0x2001);
    set_property(16,  0x4011);
    set_property(17,  0x3050);
}
//--------------------------------------------------------------------------------
void MainBox::calc_line(qreal center_x,
                        qreal center_y,
                        qreal angle,
                        qreal radius,
                        qreal *end_x,
                        qreal *end_y)
{
    qreal A = radius;
    qreal B = qCos(qDegreesToRadians(angle)) * A;
    qreal C = qSin(qDegreesToRadians(angle)) * A;

    *end_x = center_x + B;
    *end_y = center_y + C;
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_on = add_button(testbar,
                                     new QToolButton(this),
                                     QIcon(),
                                     "1",
                                     "1");
    QToolButton *btn_off = add_button(testbar,
                                      new QToolButton(this),
                                      QIcon(),
                                      "0",
                                      "0");

    QToolButton *btn_test = add_button(testbar,
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
    QGridLayout *grid = new QGridLayout();
    grid->setMargin(0);
    grid->setSpacing(0);
    for(int y=0; y<MAX_SCREEN_Y; y++)
    {
        for(int x=0; x<MAX_SCREEN_X; x++)
        {
            btn[x][y] = new QToolButton(this);
            btn[x][y]->setCheckable(true);
            btn[x][y]->setText(QString("%1:%2").arg(x).arg(y));
            btn[x][y]->setProperty(PROPERTY_X, x);
            btn[x][y]->setProperty(PROPERTY_Y, y);
            connect(btn[x][y], SIGNAL(clicked(bool)),   this,   SLOT(btn_click(bool)));
            grid_buttons.append(btn[x][y]);

            grid->addWidget(btn[x][y], x, y);
        }
    }

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addLayout(grid);
    hbox->addStretch(1);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(hbox);
    vbox->addStretch(1);

    sl_value = new QSlider(Qt::Horizontal);
    sb_value = new QSpinBox;

    sl_value->setRange(0, 100);
    sl_value->setRange(0, 100);

    connect(sl_value,   SIGNAL(valueChanged(int)),  sb_value,   SLOT(setValue(int)));

    QGridLayout *grid_btn = new QGridLayout();
    grid_btn->addWidget(new QLabel("Value:"),    0, 0);
    grid_btn->addWidget(sl_value,    0, 1);
    grid_btn->addWidget(sb_value,    0, 2);

    vbox->addLayout(grid_btn);
    vbox->addStretch(10);

    ui->frame->setLayout(vbox);
}
//--------------------------------------------------------------------------------
void MainBox::btn_click(bool state)
{
    Q_UNUSED(state)

    QToolButton *btn = (QToolButton *)sender();
    if(!btn) return;

    int x = btn->property(PROPERTY_X).toInt();
    int y = btn->property(PROPERTY_Y).toInt();

    F_01 packet;

    packet.body.header.addr = 0;
    packet.body.header.cmd = CMD_0x01;
    packet.body.header.len = sizeof(packet.body.data_t);

    buf_leds[x][y] = btn->isChecked() ? 100 : 0;

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
    emit debug(ba.toHex());

    QString send_packet = QString(":%1\n")
            .arg(ba.toHex().data());

    QByteArray o_ba;
    o_ba.clear();
    o_ba.append(send_packet);
    emit send(o_ba);
}
//--------------------------------------------------------------------------------
void MainBox::set_property(int btn_index, quint16 value)
{
    union UINT16 {
        uint16_t value;
        struct {
            uint8_t c:4;
            uint8_t d:4;
            uint8_t a:4;
            uint8_t b:4;
        } bytes;
    };

    union UINT16 temp;
    temp.value = value;

    pic_buttons.at(btn_index)->setProperty(PROPERTY_X1, temp.bytes.a);
    pic_buttons.at(btn_index)->setProperty(PROPERTY_Y1, temp.bytes.b);
    pic_buttons.at(btn_index)->setProperty(PROPERTY_X2, temp.bytes.c);
    pic_buttons.at(btn_index)->setProperty(PROPERTY_Y2, temp.bytes.d);
}
//--------------------------------------------------------------------------------
void MainBox::btn_click_adv(bool state)
{
    Q_UNUSED(state)

    QToolButton *btn = (QToolButton *)sender();
    if(!btn) return;

    int x1 = btn->property(PROPERTY_X1).toInt();
    int y1 = btn->property(PROPERTY_Y1).toInt();
    int x2 = btn->property(PROPERTY_X2).toInt();
    int y2 = btn->property(PROPERTY_Y2).toInt();

    F_01 packet;

    packet.body.header.addr = 0;
    packet.body.header.cmd = CMD_0x01;
    packet.body.header.len = sizeof(packet.body.data_t);

    buf_leds[x1][y1] = btn->isChecked() ? 100 : 0;
    buf_leds[x2][y2] = btn->isChecked() ? 100 : 0;

    for(int y=0; y<MAX_SCREEN_Y; y++)
    {
        for(int x=0; x<MAX_SCREEN_X; x++)
        {
            packet.body.data_t.leds[x][y] = buf_leds[x][y];
        }
    }

    QByteArray ba;
    ba.clear();
    ba.append((char *)&packet.buf, sizeof(packet));
    emit debug(ba.toHex());

    QString send_packet = QString(":%1\n")
            .arg(ba.toHex().data());

    QByteArray o_ba;
    o_ba.clear();
    o_ba.append(send_packet);
    emit send(o_ba);
}
//--------------------------------------------------------------------------------
void MainBox::send_grid_question(int value)
{
    foreach (QToolButton *btn, grid_buttons)
    {
        if(btn->isChecked())
        {
            int x = btn->property(PROPERTY_X).toInt();
            int y = btn->property(PROPERTY_Y).toInt();
            emit debug(QString("[%1:%2]")
                      .arg(x)
                      .arg(y));
            buf_leds[x][y] = value;
        }
    }

    //---
    F_01 packet;

    packet.body.header.addr = 0;
    packet.body.header.cmd = CMD_0x01;
    packet.body.header.len = sizeof(packet.body.data_t);

    for(int y=0; y<MAX_SCREEN_Y; y++)
    {
        for(int x=0; x<MAX_SCREEN_X; x++)
        {
            packet.body.data_t.leds[x][y] = buf_leds[x][y];
        }
    }
    QByteArray ba;
    ba.clear();
    ba.append((char *)&packet.buf, sizeof(packet));
    emit debug(ba.toHex());

    QString send_packet = QString(":%1\n")
            .arg(ba.toHex().data());

    QByteArray o_ba;
    o_ba.clear();
    o_ba.append(send_packet);
    emit send(o_ba);
}
//--------------------------------------------------------------------------------
void MainBox::send_cold_pic_question(int value)
{
    foreach (Button *btn, pic_buttons)
    {
        if(btn->isChecked())
        {
            int x1 = btn->property(PROPERTY_X1).toInt();
            int y1 = btn->property(PROPERTY_Y1).toInt();
            int x2 = btn->property(PROPERTY_X2).toInt();
            int y2 = btn->property(PROPERTY_Y2).toInt();
            emit debug(QString("[%1:%2] [%3:%4]")
                      .arg(x1)
                      .arg(y1)
                      .arg(x2)
                      .arg(y2));
            buf_leds[x1][y1] = value;
            //buf_leds[x2][y2] = value;
        }
    }

    //---
    F_01 packet;

    packet.body.header.addr = 0;
    packet.body.header.cmd = CMD_0x01;
    packet.body.header.len = sizeof(packet.body.data_t);

    for(int y=0; y<MAX_SCREEN_Y; y++)
    {
        for(int x=0; x<MAX_SCREEN_X; x++)
        {
            packet.body.data_t.leds[x][y] = buf_leds[x][y];
        }
    }

    QByteArray ba;
    ba.clear();
    ba.append((char *)&packet.buf, sizeof(packet));
    emit debug(ba.toHex());

    QString send_packet = QString(":%1\n")
            .arg(ba.toHex().data());

    QByteArray o_ba;
    o_ba.clear();
    o_ba.append(send_packet);
    emit send(o_ba);
}
//--------------------------------------------------------------------------------
void MainBox::send_hot_pic_question(int value)
{
    foreach (Button *btn, pic_buttons)
    {
        if(btn->isChecked())
        {
            int x1 = btn->property(PROPERTY_X1).toInt();
            int y1 = btn->property(PROPERTY_Y1).toInt();
            int x2 = btn->property(PROPERTY_X2).toInt();
            int y2 = btn->property(PROPERTY_Y2).toInt();
            emit debug(QString("[%1:%2] [%3:%4]")
                      .arg(x1)
                      .arg(y1)
                      .arg(x2)
                      .arg(y2));
            //buf_leds[x1][y1] = value;
            buf_leds[x2][y2] = value;
        }
    }

    //---
    F_01 packet;

    packet.body.header.addr = 0;
    packet.body.header.cmd = CMD_0x01;
    packet.body.header.len = sizeof(packet.body.data_t);

    for(int y=0; y<MAX_SCREEN_Y; y++)
    {
        for(int x=0; x<MAX_SCREEN_X; x++)
        {
            packet.body.data_t.leds[x][y] = buf_leds[x][y];
        }
    }

    QByteArray ba;
    ba.clear();
    ba.append((char *)&packet.buf, sizeof(packet));
    emit debug(ba.toHex());

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
    serialBox = new SerialBox5(this, "RS232", "RS232");
    serialBox->add_menu(2);

    ui->serial_layout->addWidget(serialBox);
    ui->serial_layout->addStretch();

    connect(this,       SIGNAL(send(QByteArray)),   serialBox,  SLOT(input(QByteArray)));
    connect(serialBox,  SIGNAL(output(QByteArray)), this,       SLOT(read_data(QByteArray)));
}
//--------------------------------------------------------------------------------
void MainBox::on(void)
{
    foreach (QToolButton *btn, grid_buttons)
    {
        btn->setChecked(true);
    }
}
//--------------------------------------------------------------------------------
void MainBox::off(void)
{
    foreach (QToolButton *btn, grid_buttons)
    {
        btn->setChecked(false);
    }
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
    emit info("test");

    foreach (Button *btn, pic_buttons)
    {
        //emit info(QString("%1").arg(btn->isChecked()));
        btn->click();
        //btn->setChecked(true);
        //btn->update();
    }
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
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
