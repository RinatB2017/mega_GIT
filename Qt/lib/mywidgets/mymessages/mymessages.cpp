/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#include "mymessages.hpp"
//--------------------------------------------------------------------------------
MyMessages::MyMessages()
{

}
//--------------------------------------------------------------------------------
int MyMessages::messagebox_noicon(const QString title,
                                const QString text,
                                unsigned int width)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::NoIcon);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Ok);
    QSpacerItem* horizontalSpacer = new QSpacerItem(static_cast<int>(width), 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = static_cast<QGridLayout *>(msgBox.layout());
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    return msgBox.exec();
}
//--------------------------------------------------------------------------------
int MyMessages::messagebox_info(const QString title,
                              const QString text,
                              unsigned int width)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Ok);
    QSpacerItem* horizontalSpacer = new QSpacerItem(static_cast<int>(width), 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = static_cast<QGridLayout *>(msgBox.layout());
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    return msgBox.exec();
}
//--------------------------------------------------------------------------------
int MyMessages::messagebox_question(const QString title,
                                  const QString text,
                                  unsigned int width)
{
    QMessageBox msgBox;

    //msgBox.setIconPixmap(QPixmap(":/qmessagebox/qmessagebox-quest.png"));
    msgBox.setIcon(QMessageBox::Question);

    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    QSpacerItem* horizontalSpacer = new QSpacerItem(static_cast<int>(width), 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = static_cast<QGridLayout *>(msgBox.layout());
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    return msgBox.exec();
}
//--------------------------------------------------------------------------------
int MyMessages::messagebox_critical(const QString title,
                                  const QString text,
                                  unsigned int width)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Ok);
    QSpacerItem* horizontalSpacer = new QSpacerItem(static_cast<int>(width), 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = static_cast<QGridLayout *>(msgBox.layout());
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    return msgBox.exec();
}
//--------------------------------------------------------------------------------
int MyMessages::messagebox_warning(const QString title,
                                 const QString text,
                                 unsigned int width)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Ok);
    QSpacerItem* horizontalSpacer = new QSpacerItem(static_cast<int>(width), 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = static_cast<QGridLayout *>(msgBox.layout());
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    return msgBox.exec();
}
//--------------------------------------------------------------------------------
