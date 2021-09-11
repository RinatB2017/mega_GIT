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
#include "ui_test_CRC_mainbox.h"
//--------------------------------------------------------------------------------
#include "myfiledialog.hpp"
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_CRC_mainbox.hpp"
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
    init();;
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
    createTestBar();
#endif

    ui->le_hex_string->setProperty(NO_SAVE, true);

    connect(ui->btn_calc,       &QPushButton::clicked,  this,   &MainBox::run_crc);
    connect(ui->btn_load_file,  &QPushButton::clicked,  this,   &MainBox::load_file);
    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test", &MainBox::test });

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

    //mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
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
    auto cmd_it = std::find_if(
        commands.begin(),
        commands.end(),
        [cmd](CMD command){ return command.cmd == cmd; }
    );
    if (cmd_it != commands.end())
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
void MainBox::run_crc(void)
{
    QString he_str = ui->le_hex_string->text();
    if(he_str.isEmpty())
    {
        emit error("HEX строка не задана");
        return;
    }

    QByteArray ba = QByteArray::fromHex(he_str.toLatin1());
    if(ba.isEmpty())
    {
        emit error("HEX строка не корректна");
        return;
    }

    show_crc(ba);
}
//--------------------------------------------------------------------------------
void MainBox::load_file(void)
{
    MyFileDialog *dlg = new MyFileDialog("test_crc32", "test_crc32", this);
    int btn = dlg->exec();
    if(btn == MyFileDialog::Accepted)
    {
        QStringList files = dlg->selectedFiles();
        QString filename = files.at(0);

        QByteArray ba;
        QFile file(filename);
        if(file.open(QIODevice::ReadOnly))
        {
            ba = file.readAll();
            show_crc(ba);
            file.close();
        }
    }
    dlg->deleteLater();
}
//--------------------------------------------------------------------------------
void MainBox::show_crc(QByteArray ba)
{
    static uint8_t  pelco_crc8 = CRC::pelco_crc8(reinterpret_cast<uint8_t *>(ba.data()), static_cast<uint8_t>(ba.length()));
    static uint8_t  crc8 = CRC::crc8(reinterpret_cast<uint8_t *>(ba.data()), static_cast<uint8_t>(ba.length()));
    static uint16_t crc16 = CRC::crc16(reinterpret_cast<uint8_t *>(ba.data()), static_cast<uint8_t>(ba.length()));
    static uint32_t crc32 = CRC::crc32(ba.data(), static_cast<uint8_t>(ba.length()));
    static uint16_t modbus_crc16 = CRC::modbus_crc16(reinterpret_cast<uint8_t *>(ba.data()), static_cast<uint8_t>(ba.length()));
    static uint16_t multimeter_crc16 = CRC::multimeter_crc16(reinterpret_cast<uint8_t *>(ba.data()), static_cast<uint8_t>(ba.length()));
    static uint16_t powersupply_crc16 = CRC::powersupply_crc16(reinterpret_cast<uint8_t *>(ba.data()), static_cast<uint8_t>(ba.length()));
    static uint16_t java_crc16 = CRC::java_crc16(reinterpret_cast<uint8_t *>(ba.data()), static_cast<uint8_t>(ba.length()));

    ui->le_pelco_crc8->setText(QString("%1").arg(pelco_crc8, 2, 16, QChar('0')).toUpper());
    ui->le_crc8->setText(QString("%1").arg(crc8, 2, 16, QChar('0')).toUpper());
    ui->le_crc16->setText(QString("%1").arg(crc16, 4, 16, QChar('0')).toUpper());
    ui->le_crc32->setText(QString("%1").arg(crc32, 8, 16, QChar('0')).toUpper());
    ui->le_modbus_crc16->setText(QString("%1").arg(modbus_crc16, 4, 16, QChar('0')).toUpper());
    ui->le_multimeter_crc16->setText(QString("%1").arg(multimeter_crc16, 4, 16, QChar('0')).toUpper());
    ui->le_powersupply_crc16->setText(QString("%1").arg(powersupply_crc16, 4, 16, QChar('0')).toUpper());
    ui->le_java_crc16->setText(QString("%1").arg(java_crc16, 4, 16, QChar('0')).toUpper());
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
