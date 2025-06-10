/*********************************************************************************
**                                                                              **
**     Copyright (C) 2025                                                       **
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
#include <QFrame>

#include "text_widget.hpp"
#include "bin_widget.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class SendBox5;
}
//--------------------------------------------------------------------------------
class SendBox5 : public QWidget
{
    Q_OBJECT

public:
    explicit SendBox5(QWidget *parent);
    virtual ~SendBox5();

    void block_interface(bool state);
    void updateText(void);

signals:
    void sendData(const QByteArray &data);

    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void state(bool);

private slots:
    void send_text_command(void);
    void send_bin_command(void);

private:
    Ui::SendBox5 *ui;

};
//--------------------------------------------------------------------------------
#endif // SENDBOX_HPP
