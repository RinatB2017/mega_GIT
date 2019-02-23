/*********************************************************************************
**                                                                              **
**     Copyright (C) 2019                                                       **
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
#include "ui_test_modbus_mainbox.h"
//--------------------------------------------------------------------------------
#include "test_modbus_mainbox.hpp"
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "crc.h"
#include "defines.hpp"
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
    save_widgets(APPNAME);
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);
    createTestBar();

    init_serial();

    //---
    connect(ui->rb_auto,    SIGNAL(toggled(bool)),  ui->cb_flag_error,  SLOT(setEnabled(bool)));
    ui->cb_flag_error->setEnabled(false);

    ui->sl_brightness->setRange(0, 0xFF);
    ui->sl_color_R->setRange(0, 0xFF);
    ui->sl_color_G->setRange(0, 0xFF);
    ui->sl_color_B->setRange(0, 0xFF);

    ui->sb_brightness->setRange(0, 0xFF);
    ui->sb_color_R->setRange(0, 0xFF);
    ui->sb_color_G->setRange(0, 0xFF);
    ui->sb_color_B->setRange(0, 0xFF);

//    ui->sb_brightness->setReadOnly(true);
//    ui->sb_color_R->setReadOnly(true);
//    ui->sb_color_G->setReadOnly(true);
//    ui->sb_color_B->setReadOnly(true);

    connect(ui->sl_brightness,  SIGNAL(valueChanged(int)),  ui->sb_brightness,  SLOT(setValue(int)));
    connect(ui->sl_color_R,     SIGNAL(valueChanged(int)),  ui->sb_color_R,     SLOT(setValue(int)));
    connect(ui->sl_color_G,     SIGNAL(valueChanged(int)),  ui->sb_color_G,     SLOT(setValue(int)));
    connect(ui->sl_color_B,     SIGNAL(valueChanged(int)),  ui->sb_color_B,     SLOT(setValue(int)));

    connect(ui->sb_brightness,  SIGNAL(valueChanged(int)),  ui->sl_brightness,  SLOT(setValue(int)));
    connect(ui->sb_color_R,     SIGNAL(valueChanged(int)),  ui->sl_color_R,     SLOT(setValue(int)));
    connect(ui->sb_color_G,     SIGNAL(valueChanged(int)),  ui->sl_color_G,     SLOT(setValue(int)));
    connect(ui->sb_color_B,     SIGNAL(valueChanged(int)),  ui->sl_color_B,     SLOT(setValue(int)));

    int index = ui->grid->rowCount();
    for(int n=0; n<10; n++)
    {
        QRadioButton *rb = new QRadioButton(this);
        rb->setObjectName(QString("rb_%1").arg(n));
        rb->setProperty("index", n);
        rb->setText(QString("%1").arg(n));

        QLineEdit *le_name = new QLineEdit(this);
        le_name->setObjectName(QString("le_name_%1").arg(n));
        le_name->setProperty("index", n);

        QLineEdit *le_answer = new QLineEdit(this);
        le_answer->setObjectName(QString("le_answer_%1").arg(n));
        le_answer->setProperty("index", n);

        connect(rb, SIGNAL(toggled(bool)),  le_name,    SLOT(setEnabled(bool)));
        connect(rb, SIGNAL(toggled(bool)),  le_answer,  SLOT(setEnabled(bool)));
        le_name->setEnabled(false);
        le_answer->setEnabled(false);

        ui->grid->addWidget(rb,          index + n, 0);
        ui->grid->addWidget(le_name,     index + n, 1);
        ui->grid->addWidget(le_answer,   index + n, 2);
    }
    //---
    connect(ui->btn_send,   SIGNAL(clicked(bool)),  this,   SLOT(send_packet()));

#if 1
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
#else
    if(sizeHint().height() > 0)
    {
        setMinimumHeight(sizeHint().height());
    }
#endif

    load_widgets(APPNAME);
}
//--------------------------------------------------------------------------------
bool MainBox::get_le_name(int index, QString *result)
{
    QList<QLineEdit *> allobj = findChildren<QLineEdit *>();
    foreach(QLineEdit *obj, allobj)
    {
        if(obj->objectName() == QString("le_name_%1").arg(index))
        {
            (*result) = obj->text();
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
bool MainBox::get_le_answer(int index, QString *result)
{
    QList<QLineEdit *> allobj = findChildren<QLineEdit *>();
    foreach(QLineEdit *obj, allobj)
    {
        if(obj->objectName() == QString("le_answer_%1").arg(index))
        {
            (*result) = obj->text();
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
void MainBox::init_serial(void)
{
    ui->serial_widget->set_fix_baudrate(115200);
    connect(this,               SIGNAL(send(QByteArray)),   ui->serial_widget,  SLOT(input(QByteArray)));
    connect(ui->serial_widget,  SIGNAL(output(QByteArray)), this,               SLOT(read_data(QByteArray)));
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &MainBox::test_0 });
    commands.append({ ID_TEST_1, "test 1", &MainBox::test_1 });
    commands.append({ ID_TEST_2, "test 2", &MainBox::test_2 });
    commands.append({ ID_TEST_3, "test 3", &MainBox::test_3 });
    commands.append({ ID_TEST_4, "test 4", &MainBox::test_4 });
    commands.append({ ID_TEST_5, "test 5", &MainBox::test_5 });
    commands.append({ ID_TEST_6, "test 6", nullptr });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok)
    {
        return;
    }
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef bool (MainBox::*my_mega_function)(void);
            my_mega_function x;
            x = command.func;
            if(x)
            {
                (this->*x)();
            }
            else
            {
                emit error("no func");
            }

            return;
        }
    }
}
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit info("Test_0()");

    send_answer();

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit info("Test_1()");

    //---
    HEADER header;
    header.address = 0;
    header.command = 1;
    header.cnt_data = 0;

    uint32_t crc32 = CRC::crc32(reinterpret_cast<char *>(&header), sizeof(HEADER));
    emit info(QString("%1").arg(crc32, 0, 16).toUpper());

    QByteArray ba;
    ba.append(reinterpret_cast<char *>(&header), sizeof(HEADER));
    ba.append(reinterpret_cast<char *>(&crc32)+3,   1);
    ba.append(reinterpret_cast<char *>(&crc32)+2,   1);
    ba.append(reinterpret_cast<char *>(&crc32)+1,   1);
    ba.append(reinterpret_cast<char *>(&crc32),     1);

    emit info(QString(":%1").arg(ba.toHex().data()).toUpper());
    /*
    :000000FF41D912
    :000100E65AE853
    :000200CD77BB90
    :000300D46C8AD1
    :0004009B2D1C16
     */
    //---

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit info("Test_2()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit info("Test_3()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit info("Test_4()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit info("Test_5()");
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray ba_data)
{
    if(ba_data.isEmpty())
    {
        return;
    }

    QString temp = QString("%1").arg(ba_data.data()).replace("\r", "").replace("\n", "");
    emit debug(QString("read_data [%1]").arg(temp));

    for(int n=0; n<ba_data.length(); n++)
    {
        char s = ba_data.at(n);
        switch(s)
        {
        case ':':
            clean_packet.clear();
            break;

        case '\r':
        case '\n':
            analize_packet();
            clean_packet.clear();
            break;

        default:
            clean_packet.append(s);
            break;
        }
    }
}
//--------------------------------------------------------------------------------
bool MainBox::check_packet(void)
{
    emit info("check_packet");

    QByteArray ba_temp;
    ba_temp.clear();
    ba_temp.append(clean_packet);

    QByteArray ba_packet;
    ba_packet.clear();
    ba_packet.append(QByteArray::fromHex(ba_temp));

    if(ba_packet.length() < static_cast<int>(sizeof(HEADER)))
    {
        emit error("packet too small");
        return false;
    }

    HEADER *header = reinterpret_cast<HEADER *>(ba_packet.data());
    int addr = header->address;
    int cmd  = header->command;
    int cnt  = header->cnt_data;

    if(ba_packet.length() < (static_cast<int>(sizeof(HEADER)) + cnt))
    {
        emit error("packet bad size");
        return false;
    }

    emit info(QString("addr: %1").arg(addr));
    emit info(QString("cmd:  %1").arg(cmd));

    return true;
}
//--------------------------------------------------------------------------------
void MainBox::analize_packet(void)
{
    emit info("analize_packet");
    if(check_packet())
    {
        send_answer();
    }
}
//--------------------------------------------------------------------------------
void MainBox::prepare_packet(void)
{
    QByteArray packet;

    QByteArray ba_temp;
    ba_temp.clear();
    ba_temp.append(clean_packet);

    QByteArray ba_packet;
    ba_packet.clear();
    ba_packet.append(QByteArray::fromHex(ba_temp));

    HEADER *i_header = reinterpret_cast<HEADER *>(ba_packet.data());
    uint8_t addr  = i_header->command;
    uint8_t cmd   = i_header->command;
    uint8_t cnt   = i_header->command;

    if(ui->cb_flag_error->isChecked())
    {
        cmd = cmd & 0x80;
    }

    //---
    HEADER o_header;
    o_header.address  = addr;
    o_header.command  = cmd;
    o_header.cnt_data = cnt;

    uint32_t crc32 = CRC::crc32(reinterpret_cast<char *>(&o_header), sizeof(HEADER));
    emit info(QString("%1").arg(crc32, 0, 16).toUpper());

    QByteArray ba;
    ba.append(reinterpret_cast<char *>(&o_header),  sizeof(HEADER));
    ba.append(reinterpret_cast<char *>(&crc32)+3,   1);
    ba.append(reinterpret_cast<char *>(&crc32)+2,   1);
    ba.append(reinterpret_cast<char *>(&crc32)+1,   1);
    ba.append(reinterpret_cast<char *>(&crc32),     1);
    //---

    packet.append(":");
    packet.append(ba.toHex().toUpper());
    packet.append(0x0D);

    if(ui->serial_widget->isOpen())
    {
        emit send(packet);
    }
}
//--------------------------------------------------------------------------------
void MainBox::send_answer(void)
{
    int index = -1;
    QString name;
    QString answer;

    if(ui->rb_auto->isChecked())
    {
        prepare_packet();
        return;
    }

    QList<QRadioButton *> allobj = findChildren<QRadioButton *>();
    foreach(QRadioButton *btn, allobj)
    {
        if(btn->isChecked())
        {
            emit debug(btn->objectName());
            index = btn->property("index").toInt();
        }
    }
    if(index < 0)
    {
        return;
    }

    get_le_name(index, &name);
    get_le_answer(index, &answer);

    emit debug(name);
    emit debug(answer);

    if(answer.isEmpty())
    {
        return;
    }

    QByteArray packet;
    packet.append(":");
    packet.append(answer);
    packet.append(0x0D);

    emit debug(packet.toHex().toUpper());

    if(ui->serial_widget->isOpen())
    {
        emit send(packet);
    }
}
//--------------------------------------------------------------------------------
void MainBox::send_packet(void)
{
    emit info("send_packet");

    HEADER header;
    header.address = static_cast<uint8_t>(ui->sb_address->value());
    header.command = static_cast<uint8_t>(ui->sb_cmd->value());
    header.cnt_data = sizeof(CMD_1);

    CMD_1 command_1;
    command_1.brightness = static_cast<uint8_t>(ui->sl_brightness->value());
    command_1.color_R = static_cast<uint8_t>(ui->sl_color_R->value());
    command_1.color_G = static_cast<uint8_t>(ui->sl_color_G->value());
    command_1.color_B = static_cast<uint8_t>(ui->sl_color_B->value());

    QByteArray ba;
    ba.append(reinterpret_cast<char *>(&header),    sizeof(HEADER));
    ba.append(reinterpret_cast<char *>(&command_1), sizeof(CMD_1));

    uint32_t crc32 = CRC::crc32(reinterpret_cast<char *>(ba.data()), static_cast<size_t>(ba.length()));

    ba.append(reinterpret_cast<char *>(&crc32)+3,   1);
    ba.append(reinterpret_cast<char *>(&crc32)+2,   1);
    ba.append(reinterpret_cast<char *>(&crc32)+1,   1);
    ba.append(reinterpret_cast<char *>(&crc32),     1);

    QByteArray packet;
    packet.append(":");
    packet.append(ba.toHex().toUpper());
    packet.append(0x0D);

    emit debug(packet);
    if(ui->serial_widget->isOpen())
    {
        emit send(packet);
    }
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
