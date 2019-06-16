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
#ifndef SENDBOX_HPP
#define SENDBOX_HPP
//--------------------------------------------------------------------------------
#include <QByteArray>
#include <QFrame>
//--------------------------------------------------------------------------------
class QGridLayout;
class QHBoxLayout;
class QToolButton;
class QLineEdit;
class QComboBox;
class QCheckBox;
//--------------------------------------------------------------------------------
class SendBox4 : public QFrame
{
    Q_OBJECT

public:
    SendBox4(QWidget *parent = nullptr);
    ~SendBox4();

    void block_interface(bool state);
    void updateText(void);

signals:
    void sendData(const QByteArray &data);

    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

private slots:
    void send_text(void);
    void send_bin(void);

private:
    QComboBox *cb_send_text;
    QComboBox *cb_send_bin;
    QComboBox *append_comboBox;

    QToolButton *btn_send_text;
    QToolButton *btn_send_bin;

    QHBoxLayout *hbox_text;
    QHBoxLayout *hbox_bin;

    QCheckBox *cb_SendStenToStep;

    QGridLayout *grid;
};
//--------------------------------------------------------------------------------
#endif // SENDBOX_HPP
