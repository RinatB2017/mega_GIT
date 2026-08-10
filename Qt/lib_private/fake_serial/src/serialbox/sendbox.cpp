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
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPixmap>
#include <QStyle>
#include <QFrame>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "sendbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
SendBox::SendBox(QWidget *parent) :
    QFrame(parent)
{
    if(parent)
    {
        connect(this, SIGNAL(info(QString)),  parent, SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parent, SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parent, SIGNAL(error(QString)));
    }

    vbox = new QVBoxLayout;

    hbox_text = new QHBoxLayout;
    hbox_text->setMargin(0);
    hbox_bin  = new QHBoxLayout;
    hbox_bin->setMargin(0);

    btn_send_text = new QToolButton(this);
    btn_send_text->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    btn_send_text->setObjectName("btn_send_text");

    btn_send_bin  = new QToolButton(this);
    btn_send_bin->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    btn_send_bin->setObjectName("btn_send_bin");

    le_send_text = new QLineEdit(this);
    le_send_text->setObjectName("le_send_text");

    le_send_bin  = new QLineEdit(this);
    le_send_bin->setObjectName("le_send_bin");

    le_send_text->setText("test");
    le_send_bin->setText("0A0B0C0D");

    append_comboBox = new QComboBox(this);
    append_comboBox->setObjectName("append_comboBox");

    hbox_text->addWidget(le_send_text);
    hbox_text->addWidget(btn_send_text);
    hbox_text->addWidget(append_comboBox);

    hbox_bin->addWidget(le_send_bin);
    hbox_bin->addWidget(btn_send_bin);

    vbox->addLayout(hbox_text);
    vbox->addLayout(hbox_bin);
    vbox->addStretch();

    connect(le_send_text, SIGNAL(returnPressed()), this, SLOT(send_text()));
    connect(le_send_bin,  SIGNAL(returnPressed()), this, SLOT(send_bin()));

    connect(btn_send_text, SIGNAL(clicked()), this, SLOT(send_text()));
    connect(btn_send_bin,  SIGNAL(clicked()), this, SLOT(send_bin()));

    setLayout(vbox);

    // setFixedWidth(300);
    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
}
//--------------------------------------------------------------------------------
SendBox::~SendBox()
{

}
//--------------------------------------------------------------------------------
void SendBox::send_text(void)
{
    QString data;
    QByteArray ba;

    data = le_send_text->text();
    if(data.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("send data"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("nothing send"));
        msgBox.exec();
        return;
    }
    ba.clear();
    ba.append(data);
    switch(append_comboBox->currentIndex())
    {
    case 0: // tr("no add")
        break;

    case 1: //  << tr("0x00")
        ba.append((char)0x00);
        break;

    case 2: //  << tr("0x0D")
        ba.append((char)0x0D);
        break;

    case 3: //  << tr("0x0A")
        ba.append((char)0x0A);
        break;

    case 4: //  << tr("0x0D 0x0A")
        ba.append((char)0x0D);
        ba.append((char)0x0A);
        break;
    }
    emit sendData(ba);
}
//--------------------------------------------------------------------------------
void SendBox::send_bin(void)
{
    QByteArray input_data;
    QByteArray output_data;

    input_data.clear();
    input_data.append(le_send_bin->text());
    if(input_data.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("send data"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("nothing send"));
        msgBox.exec();
        return;
    }

    output_data.clear();
    output_data.append(QByteArray::fromHex(input_data));

    if(input_data.toUpper() != output_data.toHex().toUpper())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("send data"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("invalid HEX-data"));
        msgBox.exec();
        return;
    }

    emit sendData(output_data);
}
//--------------------------------------------------------------------------------
void SendBox::updateText(void)
{
    btn_send_text->setText(tr("send"));
    btn_send_bin->setText(tr("send"));

    append_comboBox->clear();
    append_comboBox->addItems(QStringList() << tr("no add")
                              << tr("0x00")
                              << tr("0x0D")
                              << tr("0x0A")
                              << tr("0x0D 0x0A") );
}
//--------------------------------------------------------------------------------
