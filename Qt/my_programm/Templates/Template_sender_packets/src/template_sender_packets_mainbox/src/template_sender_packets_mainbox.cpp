/*********************************************************************************
**                                                                              **
**     Copyright (C) 2026                                                       **
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
#include "ui_template_sender_packets_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "template_sender_packets_mainbox.hpp"
#include "defines.hpp"
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
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    create_test_bar();
    show_objectNames();
#endif
    create_programm_bar();

    init_widgets();
    init_serial_fix();

    connects();

    load_widgets();
    setFixedHeight(sizeHint().height());
}
//--------------------------------------------------------------------------------
void MainBox::init_widgets(void)
{
    ui->sb_timeout_ms->setRange(1000, 0xFFFF);
}
//--------------------------------------------------------------------------------
void MainBox::init_serial_fix(void)
{
    ui->serial_widget->set_fix_baudrate(115200);
    connect(this,               static_cast<void (MainBox::*)(const QByteArray&)>(&MainBox::send),
            ui->serial_widget,  static_cast<int (SerialBox5_fix_baudrate::*)(const QByteArray&)>(&SerialBox5_fix_baudrate::input));
    connect(ui->serial_widget,  &SerialBox5_fix_baudrate::output,
            this,               &MainBox::read_data);
}
//--------------------------------------------------------------------------------
void MainBox::connects(void)
{
    connect(ui->btn_get_ID,     &QPushButton::clicked,  this,   &MainBox::get_ID);
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray ba)
{
    if(ui->serial_widget->isOpen() == false)
    {
        return;
    }

    emit debug(QString("read_data: %1")
                   .arg(ba.data()));

    for(int n=0; n<ba.length(); n++)
    {
        char s = ba.data()[n];
        switch(s)
        {
        case ':':
            input_ba.clear();
            data_is_ready = false;
            break;

        case 0x0A:
        case 0x0D:
            packet_ba.clear();
            packet_ba.append(QByteArray::fromHex(input_ba));
            data_is_ready = true;
            break;

        default:
            input_ba.append(s);
            break;
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::create_test_bar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    if(mw)
    {
        test_commands.clear(); int id = 0;
        test_commands.append({ id++, "test", &MainBox::test });

        QToolBar *testbar = new QToolBar("testbar");
        testbar->setObjectName("testbar");
        mw->addToolBar(Qt::TopToolBarArea, testbar);

        cb_test = new QComboBox(this);
        cb_test->setObjectName("cb_test");
        foreach (CMD command, test_commands)
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
    }
    else
    {
        emit error("mw not found!");
    }
}
//--------------------------------------------------------------------------------
void MainBox::create_programm_bar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    if(mw)
    {
        programm_commands.clear(); int id = 0;
        programm_commands.append({ id++,    "test", &MainBox::test });

        programm_bar = new QToolBar("programm_bar");
        programm_bar->setObjectName("programm_bar");
        mw->addToolBar(Qt::TopToolBarArea, programm_bar);

        cb_programm = new QComboBox(this);
        cb_programm->setObjectName("cb_programm");
        cb_programm->setProperty(NO_SAVE, true);
        foreach (CMD command, programm_commands)
        {
            cb_programm->addItem(command.cmd_text, QVariant(command.cmd));
        }

        programm_bar->addWidget(cb_programm);
        QToolButton *btn_choice_programm = add_button(programm_bar,
                                                      new QToolButton(this),
                                                      qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                                      "choice_programm",
                                                      "choice_programm");
        btn_choice_programm->setObjectName("btn_choice_programm");

        connect(btn_choice_programm,    &QPushButton::clicked,  this,   &MainBox::choice_programm);
    }
    else
    {
        emit error("mw not found!");
    }
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok) return;

    auto cmd_it = std::find_if(
        test_commands.begin(),
        test_commands.end(),
        [cmd](CMD command){ return command.cmd == cmd; }
        );
    if (cmd_it != test_commands.end())
    {
        typedef bool (MainBox::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (this->*x)();
        }
        else
        {
            emit error("no func");
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::choice_programm(void)
{
    bool ok = false;
    int cmd = cb_programm->itemData(cb_programm->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok) return;

    auto cmd_it = std::find_if(
        programm_commands.begin(),
        programm_commands.end(),
        [cmd](CMD command){ return command.cmd == cmd; }
        );
    if (cmd_it != programm_commands.end())
    {
        typedef bool (MainBox::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (this->*x)();
        }
        else
        {
            emit error("no func");
        }
    }
}
//--------------------------------------------------------------------------------
bool MainBox::test(void)
{
    emit info("Test");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::get_ID(void)
{
    emit trace(Q_FUNC_INFO);

    GET_ID_Q question;
    question.header.address = MY_ADDR;
    question.header.cmd = GET_ID;
    question.header.count_data = 0; //
    question.crc8 = crc8((uint8_t *)&question,
                         0,
                         sizeof(question) - sizeof(question.crc8));

    QByteArray packet;
    packet.append((char *)&question, sizeof(question));

#ifdef ANSI_PACKET
    QString temp = QString(":%1\n").arg(packet.toHex().toUpper().data());
    emit debug(QString("send: %1").arg(temp));
    f_send(temp.toLocal8Bit());
#else
    f_send(packet);
#endif

#ifdef FAST_COMMAND
    wait_msec(ui->sb_timeout_ms->value());
#else
    wait_msec(ui->sb_timeout_ms->value(), true);
#endif

    bool ok = check_answer<GET_ID_A>(GET_ID);
    if(!ok)
    {
        return false;
    }

    GET_ID_A *answer = reinterpret_cast<GET_ID_A *>(packet_ba.data());
    Q_ASSERT(answer);

    emit info(QString("ID: %1").arg(answer->data.id));
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::f_send(const QByteArray &data)
{
    if(data.isEmpty())
    {
        emit error("No data to send!");
        return;
    }
    emit info(QString("send to multimeter: %1")
                  .arg(data.data()));

    if(ui->serial_widget->isOpen())
    {
        input_ba.clear();
        packet_ba.clear();
        data_is_ready = false;
        emit send(data);
    }
}
//--------------------------------------------------------------------------------
template<typename T>
bool MainBox::check_answer(int cmd)
{
    if(packet_ba.isEmpty())
    {
        emit error("Data is empty");
        return false;
    }
    if(packet_ba.length() != sizeof(T))
    {
        emit error("Bad len answer");
        emit error(QString("packet_ba: %1").arg(packet_ba.toHex().data()));
        emit error(QString("packet_ba.length() = %1").arg(packet_ba.length()));
        emit error(QString("sizeof packet = %1").arg(sizeof(T)));
        return false;
    }

    T *answer = reinterpret_cast<T *>(packet_ba.data());

    uint16_t crc = crc8(reinterpret_cast<uint8_t *>(answer),
                        0,
                        sizeof(*answer) - sizeof(answer->crc8));
    uint16_t crc8 = answer->crc8;

    if(crc != crc8)
    {
        emit error("bad crc");
        emit error(QString("crc:  0x%1").arg(QString::number(crc, 16)));
        emit error(QString("crc8: 0x%1").arg(QString::number(crc8, 16)));
        return false;
    }

    if(answer->header.address != MY_ADDR)
    {
        emit error(QString("Bad address: %1").arg(answer->header.address));
        return false;
    }
    if(answer->header.cmd != cmd)
    {
        emit error(QString("Bad cmd: %1").arg(answer->header.cmd));
        emit error(QString("packet_ba: %1").arg(packet_ba.toHex().data()));
        return false;
    }

    return true;
}
//--------------------------------------------------------------------------------
uint8_t MainBox::crc8(uint8_t *buf,uint8_t offset, uint8_t length)
{
    uint8_t l;
    if(offset<length)
    {
        buf+=offset;
        l= length - offset;
    }
    else
    {
        l = length;
    }
    uint8_t crc = 0xff;
    int i=0;
    while (l--)
    {
        crc ^= *buf++;
        for (i = 0; i < 8; i++)
            crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
    }
    return crc;
}
//--------------------------------------------------------------------------------
void MainBox::wait_msec(int msec, bool waiting)
{
    QElapsedTimer time;

#ifdef TEST
    timeout_ms = TEST_WAIT_MS;
#endif

    emit debug(QString("Пауза %1 ms").arg(msec));
    if(msec < 1)
    {
        emit debug("timeout_ms < 1");
        return;
    }
    time.start();
    while(time.elapsed() < msec)
    {
        QCoreApplication::processEvents();
        if(!waiting)
        {
            if(data_is_ready)
            {
                if(is_silence == false)
                {
                    // emit info("Данные готовы");
                    emit info(QString("Данные готовы. Прошло %1 msec").arg(time.elapsed()));
                }
                return;
            }
        }
    }
    emit debug(QString("Прошло %1 msec").arg(msec));
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    if(ui->serial_widget->isOpen())
    {
        messagebox_warning("Предупреждение!", "Выключите порт перед выходом.");
        return false;
    }
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
