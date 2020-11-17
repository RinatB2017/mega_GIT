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
#include "ui_hc_06_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "hc_06_mainbox.hpp"
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
    Q_ASSERT(serialBox);

    ui->serial_layout->addWidget(serialBox);

    connect(this,       SIGNAL(send(QByteArray)),   serialBox,  SLOT(input(QByteArray)));
    connect(serialBox,  SIGNAL(output(QByteArray)), this,       SLOT(read_data(QByteArray)));

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    Q_ASSERT(mw);

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_command = new QComboBox(this);
    cb_command->addItem("AT",       QVariant(Qt::UserRole + AT));
    cb_command->addItem("RESET",    QVariant(Qt::UserRole + RESET));
    cb_command->addItem("VERSION",  QVariant(Qt::UserRole + VERSION));
    cb_command->addItem("ORGL",     QVariant(Qt::UserRole + ORGL));
    cb_command->addItem("ADDR",     QVariant(Qt::UserRole + ADDR));
    cb_command->addItem("NAME",     QVariant(Qt::UserRole + NAME));
    cb_command->addItem("RNAME",    QVariant(Qt::UserRole + RNAME));
    cb_command->addItem("ROLE",     QVariant(Qt::UserRole + ROLE));
    cb_command->addItem("CLASS",    QVariant(Qt::UserRole + CLASS));
    cb_command->addItem("IAC",      QVariant(Qt::UserRole + IAC));
    cb_command->addItem("INQM",     QVariant(Qt::UserRole + INQM));
    cb_command->addItem("PSWD",     QVariant(Qt::UserRole + PSWD));
    cb_command->addItem("UART",     QVariant(Qt::UserRole + UART));
    cb_command->addItem("CMODE",    QVariant(Qt::UserRole + CMODE));
    cb_command->addItem("BIND",     QVariant(Qt::UserRole + BIND));
    cb_command->addItem("POLAR",    QVariant(Qt::UserRole + POLAR));
    cb_command->addItem("PIO",      QVariant(Qt::UserRole + PIO));
    cb_command->addItem("MPIO",     QVariant(Qt::UserRole + MPIO));
    cb_command->addItem("IPSCAN",   QVariant(Qt::UserRole + IPSCAN));
    cb_command->addItem("SNIFF",    QVariant(Qt::UserRole + SNIFF));
    cb_command->addItem("SENM",     QVariant(Qt::UserRole + SENM));
    cb_command->addItem("PMSAD",    QVariant(Qt::UserRole + PMSAD));
    cb_command->addItem("RMAAD",    QVariant(Qt::UserRole + RMAAD));
    cb_command->addItem("FSAD",     QVariant(Qt::UserRole + FSAD));
    cb_command->addItem("ADCN",     QVariant(Qt::UserRole + ADCN));
    cb_command->addItem("MRAD",     QVariant(Qt::UserRole + MRAD));
    cb_command->addItem("STATE",    QVariant(Qt::UserRole + STATE));
    cb_command->addItem("INIT",     QVariant(Qt::UserRole + INIT));
    cb_command->addItem("INQ",      QVariant(Qt::UserRole + INQ));
    cb_command->addItem("INQC",     QVariant(Qt::UserRole + INQC));
    cb_command->addItem("PAIR",     QVariant(Qt::UserRole + PAIR));
    cb_command->addItem("LINK",     QVariant(Qt::UserRole + LINK));
    cb_command->addItem("DISC",     QVariant(Qt::UserRole + DISC));
    cb_command->addItem("ENSNIFF",  QVariant(Qt::UserRole + ENSNIFF));
    cb_command->addItem("EXSNIFF",  QVariant(Qt::UserRole + EXSNIFF));

    le_name = new QLineEdit();
    le_name->setText("HC-05");

    testbar->addWidget(new QLabel(tr("Command: ")));
    testbar->addWidget(cb_command);
    testbar->addWidget(new QLabel("Name: "));
    testbar->addWidget(le_name);

    QToolButton *btn_reset = add_button(testbar,
                                        new QToolButton(this),
                                        qApp->style()->standardIcon(QStyle::SP_CommandLink),
                                        "run",
                                        "run");
    Q_ASSERT(btn_reset);
    testbar->setFixedSize(testbar->sizeHint());

    connect(btn_reset, SIGNAL(clicked()), this, SLOT(run()));
}
//--------------------------------------------------------------------------------
void MainBox::run(void)
{
    emit debug(tr("run"));

    bool ok;
    int cmd = cb_command->itemData(cb_command->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;

    switch(cmd)
    {
    case AT:        command_AT();       break;
    case RESET:     command_RESET();    break;
    case VERSION:   command_VERSION();  break;
    case ORGL:      command_ORGL();     break;
    case ADDR:      command_ADDR();     break;
    case NAME:      command_NAME();     break;
    case RNAME:     command_RNAME();    break;
    case ROLE:      command_ROLE();     break;
    case CLASS:     command_CLASS();    break;
    case IAC:       command_IAC();      break;
    case INQM:      command_INQM();     break;
    case PSWD:      command_PSWD();     break;
    case UART:      command_UART();     break;
    case CMODE:     command_CMODE();    break;
    case BIND:      command_BIND();     break;
    case POLAR:     command_POLAR();    break;
    case PIO:       command_PIO();      break;
    case MPIO:      command_MPIO();     break;
    case IPSCAN:    command_IPSCAN();   break;
    case SNIFF:     command_SNIFF();    break;
    case SENM:      command_SENM();     break;
    case PMSAD:     command_PMSAD();    break;
    case RMAAD:     command_RMAAD();    break;
    case FSAD:      command_FSAD();     break;
    case ADCN:      command_ADCN();     break;
    case MRAD:      command_MRAD();     break;
    case STATE:     command_STATE();    break;
    case INIT:      command_INIT();     break;
    case INQ:       command_INQ();      break;
    case INQC:      command_INQC();     break;
    case PAIR:      command_PAIR();     break;
    case LINK:      command_LINK();     break;
    case DISC:      command_DISC();     break;
    case ENSNIFF:   command_ENSNIFF();  break;
    case EXSNIFF:   command_EXSNIFF();  break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::wait(int max_time_ms)
{
    QElapsedTimer time;
    time.start();
    while(time.elapsed() < max_time_ms)
    {
        QCoreApplication::processEvents();
        if(is_ready)
            break;
    }
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray ba)
{
    emit debug("read_data");
    emit debug(ba);
    data_rs232.append(ba);
    is_ready = true;
}
//--------------------------------------------------------------------------------
QByteArray MainBox::get_command_string(const QString &cmd)
{
    QByteArray ba;

    ba.clear();
    ba.append(cmd.toLatin1());
    //ba.append((char)0x0D);
    //ba.append((char)0x0A);

    return ba;
}
//--------------------------------------------------------------------------------
void MainBox::show_data(void)
{
    emit info(QString("%1 [%2]")
              .arg(data_rs232.data())
              .arg(data_rs232.toHex().data()));
}
//--------------------------------------------------------------------------------
void MainBox::command_AT(void)
{
    emit info("AT");

    data_rs232.clear();
    is_ready = false;

    int err = serialBox->input(get_command_string("AT"));
    if(err != SerialBox5::E_NO_ERROR)
    {
        emit error(QString("err = %1").arg(err));
        return;
    }
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_RESET(void)
{
    emit info("RESET");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+RESET"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_VERSION(void)
{
    emit info("VERSION");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+VERSION"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_ORGL(void)
{
    emit info("ORGL");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+ORGL"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_ADDR(void)
{
    emit info("ADDR");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+ADDR"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_NAME(void)
{
    emit info("NAME");

    if(le_name->text().isEmpty())
    {
        messagebox_critical("Ощибка", "Не задано имя модуля");
        return;
    }

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string(QString("AT+NAME%1").arg(le_name->text())));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_RNAME(void)
{
    emit info("RNAME");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+RNAME"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_ROLE(void)
{
    emit info("ROLE");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+ROLE"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_CLASS(void)
{
    emit info("CLASS");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+CLASS"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_IAC(void)
{
    emit info("IAC");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+IAC"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_INQM(void)
{
    emit info("INQM");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+INQM"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_PSWD(void)
{
    emit info("PSWD");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+PSWD"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_UART(void)
{
    emit info("UART");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+UART"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_CMODE(void)
{
    emit info("CMODE");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+CMODE"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_BIND(void)
{
    emit info("BIND");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+BIND"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_POLAR(void)
{
    emit info("POLAR");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+POLAR"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_PIO(void)
{
    emit info("PIO");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+PIO"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_MPIO(void)
{
    emit info("MPIO");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+MPIO"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_IPSCAN(void)
{
    emit info("IPSCAN");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+IPSCAN"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_SNIFF(void)
{
    emit info("SNIFF");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+SNIFF"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_SENM(void)
{
    emit info("SENM");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+SENM"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_PMSAD(void)
{
    emit info("PMSAD");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+PMSAD"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_RMAAD(void)
{
    emit info("RMAAD");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+RMAAD"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_FSAD(void)
{
    emit info("FSAD");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+FSAD"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_ADCN(void)
{
    emit info("ADCN");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+ADCN"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_MRAD(void)
{
    emit info("MRAD");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+MRAD"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_STATE(void)
{
    emit info("STATE");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+STATE"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_INIT(void)
{
    emit info("INIT");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+INIT"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_INQ(void)
{
    emit info("INQ");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+INQ"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_INQC(void)
{
    emit info("INQC");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+INQC"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_PAIR(void)
{
    emit info("PAIR");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+PAIR"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_LINK(void)
{
    emit info("LINK");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+LINK"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_DISC(void)
{
    emit info("DISC");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+DISC"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_ENSNIFF(void)
{
    emit info("ENSNIFF");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+ENSNIFF"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
}
//--------------------------------------------------------------------------------
void MainBox::command_EXSNIFF(void)
{
    emit info("EXSNIFF");

    data_rs232.clear();
    is_ready = false;

    serialBox->input(get_command_string("AT+EXSNIFF"));
    wait(MAX_WAIT);

    if(data_rs232.isEmpty())
    {
        emit error(tr("Нет данных"));
        return;
    }
    show_data();
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
