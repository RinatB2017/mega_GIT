/*********************************************************************************
**                                                                              **
**     Copyright (C) 2023                                                       **
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
#include <QLabel>
#include "b588_toolbar.hpp"
#include "b588_packets.hpp"
//--------------------------------------------------------------------------------
B588_toolbar::B588_toolbar(const QString &title, QWidget *parent) :
    QToolBar(parent)
{
    QToolBar::setWindowTitle(title);
    init();
}
//--------------------------------------------------------------------------------
B588_toolbar::B588_toolbar(QWidget *parent) :
    QToolBar(parent)
{
    init();
}
//--------------------------------------------------------------------------------
B588_toolbar::~B588_toolbar()
{
    if(c1) c1->deleteLater();
    if(c2) c2->deleteLater();
    if(btn_run) btn_run->deleteLater();
}
//--------------------------------------------------------------------------------
void B588_toolbar::init(void)
{
    c1 = new QComboBox();
    c2 = new QComboBox();

    btn_run = new QPushButton(this);
    btn_run->setMinimumWidth(50);
    btn_run->setText("run");

    c1->setObjectName("c1");
    c2->setObjectName("c2");

    c1->addItem("0x0XX", QVariant(Qt::UserRole + 0x00));
    c1->addItem("0x1XX", QVariant(Qt::UserRole + 0x01));
    c1->addItem("0x2XX", QVariant(Qt::UserRole + 0x02));
    c1->addItem("0x3XX", QVariant(Qt::UserRole + 0x03));
    c1->addItem("0x4XX", QVariant(Qt::UserRole + 0x04));
    c1->addItem("0x5XX", QVariant(Qt::UserRole + 0x05));
    c1->addItem("0x6XX", QVariant(Qt::UserRole + 0x06));
    c1->addItem("0x7XX", QVariant(Qt::UserRole + 0x07));

    addWidget(new QLabel(tr("Б5-88")));
    addWidget(c1);
    addWidget(c2);
    addWidget(btn_run);

    connect(c1,     static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,   &B588_toolbar::currentIndexChanged);
    connect(btn_run,    &QPushButton::released,
            this,       &B588_toolbar::run);
    currentIndexChanged(0);
}
//--------------------------------------------------------------------------------
void B588_toolbar::currentIndexChanged(int index)
{
    bool ok = false;
    int cmd = c1->itemData(index, Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;

    switch(cmd)
    {
    case 0:
        c2->clear();
        c2->addItem(B588_CMD_0x01_TEXT, QVariant(Qt::UserRole + B588_CMD_0x01));
        c2->addItem(B588_CMD_0x02_TEXT, QVariant(Qt::UserRole + B588_CMD_0x02));
        c2->addItem(B588_CMD_0x03_TEXT, QVariant(Qt::UserRole + B588_CMD_0x03));
        c2->addItem(B588_CMD_0x04_TEXT, QVariant(Qt::UserRole + B588_CMD_0x04));
        c2->addItem(B588_CMD_0x05_TEXT, QVariant(Qt::UserRole + B588_CMD_0x05));
        c2->addItem(B588_CMD_0x06_TEXT, QVariant(Qt::UserRole + B588_CMD_0x06));
        c2->addItem(B588_CMD_0x07_TEXT, QVariant(Qt::UserRole + B588_CMD_0x07));
        c2->addItem(B588_CMD_0x08_TEXT, QVariant(Qt::UserRole + B588_CMD_0x08));
        c2->addItem(B588_CMD_0x09_TEXT, QVariant(Qt::UserRole + B588_CMD_0x09));
        c2->addItem(B588_CMD_0x0A_TEXT, QVariant(Qt::UserRole + B588_CMD_0x0A));
        c2->addItem(B588_CMD_0x0B_TEXT, QVariant(Qt::UserRole + B588_CMD_0x0B));
        c2->addItem(B588_CMD_0x0C_TEXT, QVariant(Qt::UserRole + B588_CMD_0x0C));
        c2->addItem(B588_CMD_0x0D_TEXT, QVariant(Qt::UserRole + B588_CMD_0x0D));
        c2->addItem(B588_CMD_0x0E_TEXT, QVariant(Qt::UserRole + B588_CMD_0x0E));
        c2->addItem(B588_CMD_0x0F_TEXT, QVariant(Qt::UserRole + B588_CMD_0x0F));
        break;

    case 1:
        c2->clear();
        c2->addItem(B588_CMD_0x10_TEXT, QVariant(Qt::UserRole + B588_CMD_0x10));
        c2->addItem(B588_CMD_0x11_TEXT, QVariant(Qt::UserRole + B588_CMD_0x11));
        c2->addItem(B588_CMD_0x12_TEXT, QVariant(Qt::UserRole + B588_CMD_0x12));
        c2->addItem(B588_CMD_0x13_TEXT, QVariant(Qt::UserRole + B588_CMD_0x13));
        c2->addItem(B588_CMD_0x14_TEXT, QVariant(Qt::UserRole + B588_CMD_0x14));
        c2->addItem(B588_CMD_0x15_TEXT, QVariant(Qt::UserRole + B588_CMD_0x15));
        c2->addItem(B588_CMD_0x16_TEXT, QVariant(Qt::UserRole + B588_CMD_0x16));
        c2->addItem(B588_CMD_0x17_TEXT, QVariant(Qt::UserRole + B588_CMD_0x17));
        break;

    case 2:
        c2->clear();
        c2->addItem(B588_CMD_0x20_TEXT, QVariant(Qt::UserRole + B588_CMD_0x20));
        c2->addItem(B588_CMD_0x21_TEXT, QVariant(Qt::UserRole + B588_CMD_0x21));
        c2->addItem(B588_CMD_0x22_TEXT, QVariant(Qt::UserRole + B588_CMD_0x22));
        c2->addItem(B588_CMD_0x23_TEXT, QVariant(Qt::UserRole + B588_CMD_0x23));
        c2->addItem(B588_CMD_0x24_TEXT, QVariant(Qt::UserRole + B588_CMD_0x24));
        c2->addItem(B588_CMD_0x25_TEXT, QVariant(Qt::UserRole + B588_CMD_0x25));
        c2->addItem(B588_CMD_0x26_TEXT, QVariant(Qt::UserRole + B588_CMD_0x26));
        c2->addItem(B588_CMD_0x27_TEXT, QVariant(Qt::UserRole + B588_CMD_0x27));
        c2->addItem(B588_CMD_0x28_TEXT, QVariant(Qt::UserRole + B588_CMD_0x28));
        c2->addItem(B588_CMD_0x29_TEXT, QVariant(Qt::UserRole + B588_CMD_0x29));
        c2->addItem(B588_CMD_0x2A_TEXT, QVariant(Qt::UserRole + B588_CMD_0x2A));
        c2->addItem(B588_CMD_0x2B_TEXT, QVariant(Qt::UserRole + B588_CMD_0x2B));
        c2->addItem(B588_CMD_0x2C_TEXT, QVariant(Qt::UserRole + B588_CMD_0x2C));
        c2->addItem(B588_CMD_0x2D_TEXT, QVariant(Qt::UserRole + B588_CMD_0x2D));
        c2->addItem(B588_CMD_0x2E_TEXT, QVariant(Qt::UserRole + B588_CMD_0x2E));
        c2->addItem(B588_CMD_0x2F_TEXT, QVariant(Qt::UserRole + B588_CMD_0x2F));
        break;

    case 3:
        c2->clear();
        c2->addItem(B588_CMD_0x30_TEXT, QVariant(Qt::UserRole + B588_CMD_0x30));
        c2->addItem(B588_CMD_0x31_TEXT, QVariant(Qt::UserRole + B588_CMD_0x31));
        c2->addItem(B588_CMD_0x32_TEXT, QVariant(Qt::UserRole + B588_CMD_0x32));
        c2->addItem(B588_CMD_0x33_TEXT, QVariant(Qt::UserRole + B588_CMD_0x33));
        c2->addItem(B588_CMD_0x34_TEXT, QVariant(Qt::UserRole + B588_CMD_0x34));
        c2->addItem(B588_CMD_0x35_TEXT, QVariant(Qt::UserRole + B588_CMD_0x35));
        c2->addItem(B588_CMD_0x36_TEXT, QVariant(Qt::UserRole + B588_CMD_0x36));
        c2->addItem(B588_CMD_0x37_TEXT, QVariant(Qt::UserRole + B588_CMD_0x37));
        c2->addItem(B588_CMD_0x38_TEXT, QVariant(Qt::UserRole + B588_CMD_0x38));
        c2->addItem(B588_CMD_0x39_TEXT, QVariant(Qt::UserRole + B588_CMD_0x39));
        c2->addItem(B588_CMD_0x3A_TEXT, QVariant(Qt::UserRole + B588_CMD_0x3A));
        c2->addItem(B588_CMD_0x3B_TEXT, QVariant(Qt::UserRole + B588_CMD_0x3B));
        c2->addItem(B588_CMD_0x3C_TEXT, QVariant(Qt::UserRole + B588_CMD_0x3C));
        c2->addItem(B588_CMD_0x3D_TEXT, QVariant(Qt::UserRole + B588_CMD_0x3D));
        c2->addItem(B588_CMD_0x3E_TEXT, QVariant(Qt::UserRole + B588_CMD_0x3E));
        c2->addItem(B588_CMD_0x3F_TEXT, QVariant(Qt::UserRole + B588_CMD_0x3F));
        break;

    case 4:
        c2->clear();
        c2->addItem(B588_CMD_0x41_TEXT, QVariant(Qt::UserRole + B588_CMD_0x41));
        c2->addItem(B588_CMD_0x42_TEXT, QVariant(Qt::UserRole + B588_CMD_0x42));
        c2->addItem(B588_CMD_0x43_TEXT, QVariant(Qt::UserRole + B588_CMD_0x43));
        c2->addItem(B588_CMD_0x44_TEXT, QVariant(Qt::UserRole + B588_CMD_0x44));
        c2->addItem(B588_CMD_0x46_TEXT, QVariant(Qt::UserRole + B588_CMD_0x46));
        c2->addItem(B588_CMD_0x47_TEXT, QVariant(Qt::UserRole + B588_CMD_0x47));
        c2->addItem(B588_CMD_0x48_TEXT, QVariant(Qt::UserRole + B588_CMD_0x48));
        c2->addItem(B588_CMD_0x49_TEXT, QVariant(Qt::UserRole + B588_CMD_0x49));
        c2->addItem(B588_CMD_0x4A_TEXT, QVariant(Qt::UserRole + B588_CMD_0x4A));
        break;

    case 5:
        c2->clear();
        c2->addItem(B588_CMD_0x54_TEXT, QVariant(Qt::UserRole + B588_CMD_0x54));
        c2->addItem(B588_CMD_0x55_TEXT, QVariant(Qt::UserRole + B588_CMD_0x55));
        c2->addItem(B588_CMD_0x56_TEXT, QVariant(Qt::UserRole + B588_CMD_0x56));
        c2->addItem(B588_CMD_0x57_TEXT, QVariant(Qt::UserRole + B588_CMD_0x57));
        c2->addItem(B588_CMD_0x58_TEXT, QVariant(Qt::UserRole + B588_CMD_0x58));
        c2->addItem(B588_CMD_0x59_TEXT, QVariant(Qt::UserRole + B588_CMD_0x59));
        c2->addItem(B588_CMD_0x5A_TEXT, QVariant(Qt::UserRole + B588_CMD_0x5A));
        c2->addItem(B588_CMD_0x5B_TEXT, QVariant(Qt::UserRole + B588_CMD_0x5B));
        c2->addItem(B588_CMD_0x5C_TEXT, QVariant(Qt::UserRole + B588_CMD_0x5C));
        c2->addItem(B588_CMD_0x5D_TEXT, QVariant(Qt::UserRole + B588_CMD_0x5D));
        c2->addItem(B588_CMD_0x5E_TEXT, QVariant(Qt::UserRole + B588_CMD_0x5E));
        c2->addItem(B588_CMD_0x5F_TEXT, QVariant(Qt::UserRole + B588_CMD_0x5F));
        break;

    case 6:
        c2->clear();
        c2->addItem(B588_CMD_0x60_TEXT, QVariant(Qt::UserRole + B588_CMD_0x60));
        c2->addItem(B588_CMD_0x61_TEXT, QVariant(Qt::UserRole + B588_CMD_0x61));
        c2->addItem(B588_CMD_0x62_TEXT, QVariant(Qt::UserRole + B588_CMD_0x62));
        c2->addItem(B588_CMD_0x63_TEXT, QVariant(Qt::UserRole + B588_CMD_0x63));
        c2->addItem(B588_CMD_0x64_TEXT, QVariant(Qt::UserRole + B588_CMD_0x64));
        c2->addItem(B588_CMD_0x65_TEXT, QVariant(Qt::UserRole + B588_CMD_0x65));
        c2->addItem(B588_CMD_0x66_TEXT, QVariant(Qt::UserRole + B588_CMD_0x66));
        c2->addItem(B588_CMD_0x67_TEXT, QVariant(Qt::UserRole + B588_CMD_0x67));
        c2->addItem(B588_CMD_0x68_TEXT, QVariant(Qt::UserRole + B588_CMD_0x68));
        c2->addItem(B588_CMD_0x69_TEXT, QVariant(Qt::UserRole + B588_CMD_0x69));
        c2->addItem(B588_CMD_0x6A_TEXT, QVariant(Qt::UserRole + B588_CMD_0x6A));
        c2->addItem(B588_CMD_0x6B_TEXT, QVariant(Qt::UserRole + B588_CMD_0x6B));
        c2->addItem(B588_CMD_0x6C_TEXT, QVariant(Qt::UserRole + B588_CMD_0x6C));
        c2->addItem(B588_CMD_0x6D_TEXT, QVariant(Qt::UserRole + B588_CMD_0x6D));
        c2->addItem(B588_CMD_0x6E_TEXT, QVariant(Qt::UserRole + B588_CMD_0x6E));
        c2->addItem(B588_CMD_0x6F_TEXT, QVariant(Qt::UserRole + B588_CMD_0x6F));
        break;

    case 7:
        c2->clear();
        c2->addItem(B588_CMD_0x70_TEXT, QVariant(Qt::UserRole + B588_CMD_0x70));
        c2->addItem(B588_CMD_0x71_TEXT, QVariant(Qt::UserRole + B588_CMD_0x71));
        c2->addItem(B588_CMD_0x72_TEXT, QVariant(Qt::UserRole + B588_CMD_0x72));
        c2->addItem(B588_CMD_0x73_TEXT, QVariant(Qt::UserRole + B588_CMD_0x73));
        c2->addItem(B588_CMD_0x75_TEXT, QVariant(Qt::UserRole + B588_CMD_0x75));
        c2->addItem(B588_CMD_0x7B_TEXT, QVariant(Qt::UserRole + B588_CMD_0x7B));
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void B588_toolbar::run(void)
{
    bool ok = false;
    int cmd = c2->itemData(c2->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;

    emit press(cmd);
}
//--------------------------------------------------------------------------------
