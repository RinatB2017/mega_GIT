/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#ifndef FILTERBOX_HPP
#define FILTERBOX_HPP
//--------------------------------------------------------------------------------
#include <stdint.h>
//--------------------------------------------------------------------------------
#include <QSpinBox>
#include <QDialog>
#include <QList>
//--------------------------------------------------------------------------------
namespace Ui {
    class FilterBox;
}
//--------------------------------------------------------------------------------
class QTreeWidgetItem;
class Client;
class QTimer;
//--------------------------------------------------------------------------------
struct PARAM_FILTER
{
    uint16_t id;
    QString  text;
    QSpinBox *sb_value;
};
//--------------------------------------------------------------------------------
class FilterBox : public QDialog
{
    Q_OBJECT

public:
    explicit FilterBox(Client *client, QWidget *parent);
    ~FilterBox();

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void message(const QString &);

private slots:

private:
    Ui::FilterBox *ui;
    QWidget *parent;

    QList<PARAM_FILTER> params;
    Client *client;

    void init(void);
    void block_this_button(bool state);
    void block_interface(bool state);
};
//--------------------------------------------------------------------------------
#endif
