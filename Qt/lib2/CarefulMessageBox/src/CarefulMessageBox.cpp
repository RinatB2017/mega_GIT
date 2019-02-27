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
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QVariant>
#include <QLabel>
//--------------------------------------------------------------------------------
#include "CarefulMessageBox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
CarefulMessageBox::CarefulMessageBox(const QString &caption,
                                     const QString &message,
                                     QWidget *parent) :
    QDialog(parent)
{
    QLabel *msg_label = new QLabel(message);

    QVBoxLayout *vbox = new QVBoxLayout;
    QHBoxLayout *buttons_box = new QHBoxLayout;

    buttons_box->addStretch();
    for(unsigned int n=0; n<MAX_BUTTONS; n++)
    {
        buttons[n] = new QPushButton(this);
        buttons[n]->setText(QString("%1").arg(n));
        if(n<=9)
            buttons[n]->setShortcut(n + 0x30);
        buttons[n]->setProperty("name", QVariant(n));
        connect(buttons[n], SIGNAL(clicked()), this, SLOT(click()));

        buttons_box->addWidget(buttons[n]);
    }
    buttons_box->addStretch();

    true_button = rand() % MAX_BUTTONS;
    QLabel *help_label = new QLabel(QString(tr("<b>Нажмите кнопку №%1 для продолжения</b>")).arg(true_button));
    help_label->setAlignment(Qt::AlignCenter);

    vbox->addWidget(msg_label);
    vbox->addWidget(help_label);
    vbox->addLayout(buttons_box);
    setLayout(vbox);

    setWindowTitle(caption);
}
//--------------------------------------------------------------------------------
void CarefulMessageBox::click(void)
{
    QPushButton *button = (QPushButton *)sender();
    bool ok;
    int btn = button->property("name").toInt(&ok);
    if(btn == true_button)
        accept();
    else
        reject();
}
//--------------------------------------------------------------------------------
