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
#include <QHBoxLayout>
#include <QPushButton>
#include <QStringList>
#include <QByteArray>
#include <QComboBox>
#include <QLabel>
//--------------------------------------------------------------------------------
#include "tcp_client.hpp"
#include "debugframe.hpp"

#include "property_editor.hpp"
//--------------------------------------------------------------------------------
DebugFrame::DebugFrame(TCP_Client *client, QWidget *parent) :
    MyWidget(parent),
    parent(parent),
    client(client)
{
    hbox = new QHBoxLayout(this);

    lbl_table = new QLabel("table");
    lbl_takt = new QLabel("takt");
    lbl_cmd = new QLabel("cmd");

    cmb_table = new QComboBox();
    cmb_table->addItems(QStringList() << "1" << "2" << "3" << "4" << "5"
                        << "6" << "7" << "8" << "9" << "10"
                        << "11" << "12" << "13" << "14");

    cmb_takt = new QComboBox();
    cmb_takt->addItems(QStringList() << "1" << "2" << "3" << "4" << "5"
                       << "6" << "7" << "8");

    cmb_cmd = new QComboBox();
    cmb_cmd->addItems(QStringList() << "no cmd" << "0xF0" << "0xAA");

    btnDebug = new QPushButton(this);
    btnDebug->setText(tr("test"));

    hbox->addWidget(lbl_table);
    hbox->addWidget(cmb_table);

    hbox->addWidget(lbl_takt);
    hbox->addWidget(cmb_takt);

    hbox->addWidget(lbl_cmd);
    hbox->addWidget(cmb_cmd);

    hbox->addWidget(btnDebug);
    hbox->addStretch();

    setLayout(hbox);

    //setFrameStyle(QFrame::StyledPanel | QFrame::Raised);

    connect(btnDebug, SIGNAL(clicked()), this, SLOT(test()));
}
//--------------------------------------------------------------------------------
int DebugFrame::create_ba(QByteArray *data,
                          unsigned char table,
                          unsigned char takt,
                          unsigned char cmd)
{
    QByteArray ba;
    emit debug(QString("build_data: CMD_0x%1 takt %2 cmd 0x%3")
               .arg(table, 0, 16)
               .arg(takt, 0, 16)
               .arg(cmd));
    ba.clear();
    ba.append(static_cast<unsigned char>(0x55));    // marker
    ba.append(static_cast<char>(0xA9));

    ba.append(static_cast<unsigned char>(2));       // length
    ba.append(static_cast<unsigned char>(0));

    ba.append(static_cast<unsigned char>(0x01));    // address
    ba.append(static_cast<unsigned char>(0x02));    // usecase_ba
    ba.append(static_cast<unsigned char>(0x00));    // null_byte

    switch(table)
    {
    case 1:  table = 0x10; break;
    case 2:  table = 0x18; break;
    case 3:  table = 0x20; break;
    case 4:  table = 0x28; break;
    case 5:  table = 0x2B; break;
    case 6:  table = 0x2E; break;
    case 7:  table = 0x31; break;
    case 8:  table = 0x34; break;
    case 9:  table = 0x36; break;
    case 10: table = 0x39; break;
    case 11: table = 0x3B; break;
    case 12: table = 0x3E; break;
    case 13: table = 0x40; break;
    case 14: table = 0x49; break;
    }
    table += (takt - 1);

    ba.append(static_cast<char>(table));  // code_command

    switch(cmd)
    {
    case 1:  cmd = 0xF0; break;
    case 2:  cmd = 0xAA; break;
    default: cmd = 0;    break;
    }

    ba.append(static_cast<char>(cmd));

    for(int n=0; n<254; n++)
        ba.append(static_cast<unsigned char>(0x00));

    *data = ba;

    return 0;
}
//--------------------------------------------------------------------------------
void DebugFrame::test(void)
{
#if 0
    QByteArray data;
    uchar table = 0;
    uchar takt  = 0;
    uchar cmd   = 0;

    table = cmb_table->currentIndex()+1;
    takt  = cmb_takt->currentIndex()+1;
    cmd   = cmb_cmd->currentIndex();

    emit debug(QString("table %1 takt %2 cmd %3")
               .arg(table)
               .arg(takt)
               .arg(cmd));

    data.clear();
    create_ba(&data,
              table,
              takt,
              cmd);
    if(data.isEmpty())
    {
        emit error("data is empty!");
        return;
    }

    emit debug(data.toHex().toUpper());
    client->input(data);
#else
    PropertyEditor *editor = new PropertyEditor();
    editor->setColumnWidth(0, 150);
    editor->addLineProperty("Устройство", "/dev/ttyS0");
    editor->addLineProperty("Скорость", "9600 bit");
    editor->addComboProperty("combo", QStringList() << "1" << "2" << "3", "1");
    editor->addCheckProperty("check", Qt::Checked);
    editor->show();
#endif
}
//--------------------------------------------------------------------------------
