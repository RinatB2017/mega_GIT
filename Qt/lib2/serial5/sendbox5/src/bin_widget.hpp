/*********************************************************************************
**                                                                              **
**     Copyright (C) 2021                                                       **
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
#ifndef BIN_WIDGET_HPP
#define BIN_WIDGET_HPP
//--------------------------------------------------------------------------------
#include <QInputDialog>
#include <QDialog>

#include "mymessages.hpp"
#include "mysettings.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#define P_BIN_WIDGET           "bin_widget"
#define P_BIN_WIDGET_GEOMETRY  "bin_widget_geometry"
#define P_INTERVAL_BIN         "interval_bin"
//--------------------------------------------------------------------------------
namespace Ui {
    class Bin_widget;
}
//--------------------------------------------------------------------------------
class Bin_widget : public QDialog, public MySettings, public MyMessages
{
    Q_OBJECT

signals:
    void send_command(const QByteArray &);

    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

public:
    explicit Bin_widget(QWidget *parent = nullptr);
    virtual ~Bin_widget();

private:
    Ui::Bin_widget *ui;

    QTimer *timer = nullptr;

    void init(void);
    void init_timer(void);

    void append(void);
    void remove(void);
    void edit(void);
    void run(void);
    void up(void);
    void down(void);

    void f_start(void);
    void f_stop(void);
    void f_update(void);

    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // BIN_WIDGET_HPP
