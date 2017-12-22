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
//--------------------------------------------------------------------------------
#ifndef DEBUGFRAME_H
#define DEBUGFRAME_H
//--------------------------------------------------------------------------------
#include <QFrame>

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class QPushButton;
class QHBoxLayout;
class QByteArray;
class QComboBox;
class TCP_Client;
class QLabel;
//--------------------------------------------------------------------------------
class DebugFrame : public MyWidget
{
    Q_OBJECT
public:
    explicit DebugFrame(TCP_Client *client, QWidget *parent = 0);
    
signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

private slots:
    void test(void);

private:
    QWidget *parent;
    TCP_Client *client;
    QPushButton *btnDebug;
    QHBoxLayout *hbox;
    QLabel *lbl_table;
    QComboBox *cmb_table;
    QLabel *lbl_takt;
    QComboBox *cmb_takt;
    QLabel *lbl_cmd;
    QComboBox *cmb_cmd;

    int create_ba(QByteArray *data,
                  unsigned char table,
                  unsigned char takt,
                  unsigned char cmd);
};
//--------------------------------------------------------------------------------
#endif
