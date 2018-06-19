/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#include "ui_for_tests_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mymainwindow.hpp"
#include "for_tests_mainbox.hpp"
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
    if(test_widget)
    {
        test_widget->close();
        test_widget->deleteLater();
    }
    save_widgets("for_test");

    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

    connect(ui->sb_1,   SIGNAL(valueChanged(int)),  this,   SLOT(check_in()));
    connect(ui->sb_2,   SIGNAL(valueChanged(int)),  this,   SLOT(check_in()));
    connect(ui->sb_res, SIGNAL(valueChanged(int)),  this,   SLOT(check_in()));
    connect(ui->btn_ok, SIGNAL(clicked(bool)),      this,   SLOT(victory()));
    check_in();

#if 1
    //setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
#else
    if(sizeHint().height() > 0)
    {
        setMinimumHeight(sizeHint().height());
    }
#endif
    load_widgets("for_test");
}
//--------------------------------------------------------------------------------
void MainBox::check_in(void)
{
    int a = ui->sb_1->value();
    int b = ui->sb_2->value();
    int c = ui->sb_res->value();

    bool res = ((a + b) == c);
    ui->btn_ok->setEnabled(res);
}
//--------------------------------------------------------------------------------
void MainBox::victory(void)
{
    messagebox_info("Info", "Victory!");
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
    commands.append({ ID_TEST_6, "test 6", 0 });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QCheckBox *cb_block = new QCheckBox("block");
    cb_block->setObjectName("cb_block");
    testbar->addWidget(cb_block);

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

    connect(cb_block, SIGNAL(clicked(bool)), cb_test,           SLOT(setDisabled(bool)));
    connect(cb_block, SIGNAL(clicked(bool)), btn_choice_test,   SLOT(setDisabled(bool)));
}
//--------------------------------------------------------------------------------
#if 0
//TODO как нибудь потом
bool MainBox::no_exit(void)
{
    return false;
}
#endif
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok) return;
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef bool (MainBox::*function)(void);
            function x;
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
void MainBox::inFunc(QPushButton *btn, saveSlot slot)
{
    connect(btn,    &QPushButton::clicked,  this,   slot);
}
//--------------------------------------------------------------------------------
void MainBox::s_inFunc(void)
{
    emit trace(Q_FUNC_INFO);
    QMessageBox::information(0,"","info");
}
//--------------------------------------------------------------------------------
int MainBox::get_cnt(void)
{
    emit trace(Q_FUNC_INFO);
    return qrand() % 10;
}
//--------------------------------------------------------------------------------
void MainBox::test_validator(void)
{
    /* Create a string for a regular expression */
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    /* Create a regular expression with a string
     * as a repeating element
     */
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    /* Create a validation regular expression
     * using a regular expression
     */
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);

    /* Set Validator on QLineEdit */
    QLineEdit *lineEdit = new QLineEdit;
    lineEdit->setValidator(ipValidator);
    lineEdit->show();
}
//--------------------------------------------------------------------------------
#if 0
<TICKER>,<DTYYYYMMDD>,<TIME>,<OPEN>,<HIGH>,<LOW>,<CLOSE>
EURUSD,20180601,000100,1.1690,1.1690,1.1690,1.1690
EURUSD,20180601,000200,1.1690,1.1691,1.1690,1.1691
EURUSD,20180601,000300,1.1691,1.1691,1.1691,1.1691
EURUSD,20180601,000400,1.1691,1.1691,1.1691,1.1691
EURUSD,20180601,000500,1.1691,1.1692,1.1691,1.1692
EURUSD,20180601,000600,1.1692,1.1692,1.1692,1.1692
EURUSD,20180601,000700,1.1692,1.1692,1.1692,1.1692
EURUSD,20180601,000800,1.1692,1.1692,1.1692,1.1692
EURUSD,20180601,000900,1.1692,1.1692,1.1692,1.1692
EURUSD,20180601,001000,1.1693,1.1693,1.1693,1.1693
EURUSD,20180601,001100,1.1693,1.1693,1.1693,1.1693
EURUSD,20180601,001200,1.1693,1.1693,1.1693,1.1693
EURUSD,20180601,001300,1.1693,1.1693,1.1692,1.1692
EURUSD,20180601,001400,1.1692,1.1693,1.1692,1.1693
EURUSD,20180601,001500,1.1693,1.1693,1.1693,1.1693
EURUSD,20180601,001600,1.1693,1.1693,1.1693,1.1693
EURUSD,20180601,001700,1.1693,1.1693,1.1693,1.1693
EURUSD,20180601,001800,1.1693,1.1693,1.1693,1.1693
EURUSD,20180601,001900,1.1693,1.1693,1.1693,1.1693
EURUSD,20180601,002000,1.1693,1.1693,1.1693,1.1693
EURUSD,20180601,002100,1.1694,1.1695,1.1694,1.1695
EURUSD,20180601,002200,1.1696,1.1696,1.1696,1.1696
EURUSD,20180601,002300,1.1695,1.1697,1.1695,1.1696
EURUSD,20180601,002400,1.1697,1.1697,1.1696,1.1696
#endif
//--------------------------------------------------------------------------------
#include "test_protocol.hpp"
#include "crc.h"
#include <unistd.h>

#include <QDate>
#include <QTime>

bool MainBox::test_0(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_0()");

#if 0
    QTime t_time(0, 0, 0);
    QTime t2 = t_time.addSecs(2300);
    emit info(QString("%1:%2:%3")
              .arg(t2.hour())
              .arg(t2.minute())
              .arg(t2.second()));
#endif

#if 1
    bool ok = false;
    QString temp = "20180601";
    int year = 0;
    int month = 0;
    int day = 0;
    if(temp.length() == 8)
    {
        year = temp.left(4).toInt(&ok);
        month = temp.mid(4, 2).toInt(&ok);
        day = temp.right(2).toInt(&ok);
        emit info(QString("%1 %2 %3")
                  .arg(year)
                  .arg(month)
                  .arg(day));
    }

    temp = "002300";
    if(temp.length() == 6)
    {
        int t_time = temp.toInt(&ok);
        emit info(QString("t_time: %1").arg(t_time));

        QDateTime dt;
#if 1
        QDate date(year, month, day);
        dt.setDate(date);
        QDateTime dt2 = dt.addSecs(t_time);
#else
        dt.addYears(year);
        dt.addMonths(month);
        dt.addDays(day);
        dt.addSecs(t_time);
#endif
        emit info(dt2.toString("yyyy.MM.dd HH:mm:ss"));
        emit info(QString("Unixtime: %1").arg(dt2.toTime_t()));
    }
#endif

#if 0
    QDateTime dt;
    QDate date;
    date.setDate(1970, 1, 2);
    QTime time(0, 0, 0);

    dt.setDate(date);
    dt.setTime(time);

    emit info(QString("Timestamp1 is: %1").arg(dt.toTime_t()));

    //uint timestamp=QDateTime::currentDateTime().toTime_t();
    //emit info(QString("Current timestamp is: %1").arg(timestamp));

    QDateTime time2 = QDateTime::fromString (QString("1970-01-02T00:00:00"), Qt::ISODate);
    emit info(QString("Timestamp2 is: %1").arg(time2.toTime_t()));

#endif

#if 0
    QByteArray question;
    QByteArray answer;

    HEADER header;
    DATA_CMD_1 data;

    header.addr = 0;
    header.cmd = CMD_1;
    header.len = sizeof(DATA_CMD_1);

    question.append((char *)&header,    sizeof(HEADER));
    question.append((char *)&data,      sizeof(data));
    uint16_t crc16 = CRC::crc16((uint8_t *)question.data(), question.length());
    question.append((char *)&crc16,     sizeof(crc16));

    Test_protocol *proto = new Test_protocol;
    connect(proto,  SIGNAL(info(QString)),  this,   SIGNAL(info(QString)));
    connect(proto,  SIGNAL(debug(QString)), this,   SIGNAL(debug(QString)));
    connect(proto,  SIGNAL(error(QString)), this,   SIGNAL(error(QString)));
    connect(proto,  SIGNAL(trace(QString)), this,   SIGNAL(trace(QString)));

    int result = proto->check_packet(question, &answer);
    if(result != Base_protocol::E_NO_ERROR)
    {
        switch(result)
        {
        case Base_protocol::E_PACKET_EMPTY: emit error("E_PACKET_EMPTY");   break;
        case Base_protocol::E_BAD_ADDRESS:  emit error("E_BAD_ADDRESS");    break;
        case Base_protocol::E_BAD_CMD:      emit error("E_BAD_CMD");        break;
        case Base_protocol::E_BAD_DATA:     emit error("E_BAD_DATA");       break;
        case Base_protocol::E_BAD_SIZE:     emit error("E_BAD_SIZE");       break;
        case Base_protocol::E_BAD_CRC16:    emit error("E_BAD_CRC16");      break;
        default:
            emit error(QString("unknown error %1").arg(result));
            break;
        }
        return false;
    }
    emit info("OK");
#endif

#if 0
    check_tooltips();
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_1()");

#if 0
    emit info("info");
    emit debug("debug");
    emit error("error");
    emit trace("trace");

    emit colorLog("Cyan", QColor(Qt::cyan), QColor(Qt::black));
    emit colorLog("Yellow", QColor(Qt::yellow), QColor(Qt::white));
#endif

#if 0
    emit info(__FILE__);
    emit info(QString("line %1").arg(__LINE__));
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_2()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_3()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_4()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_5()");

    return true;
}
//--------------------------------------------------------------------------------
quint32 MainBox::test(const QByteArray ba)
{
    emit trace(Q_FUNC_INFO);
    quint32 temp = 0;

    for(int n=0; n<ba.length(); n++)
    {
        temp += (char)ba.at(n);
    }

    return temp;
}
//--------------------------------------------------------------------------------
