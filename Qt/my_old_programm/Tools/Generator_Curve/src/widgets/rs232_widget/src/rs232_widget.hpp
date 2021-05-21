/*********************************************************************************
**                                                                              **
**     Copyright (C) 2019                                                       **
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
#ifndef RS232_WIDGET_HPP
#define RS232_WIDGET_HPP
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class RS232_widget;
}
//--------------------------------------------------------------------------------
class Generator_Curve;
//--------------------------------------------------------------------------------
class RS232_widget : public MyWidget
{
    Q_OBJECT

signals:
    void send(QByteArray);

public:
    explicit RS232_widget(QWidget *parent = nullptr);
    virtual ~RS232_widget();

    void set_generator(Generator_Curve *gen);

private slots:
    void start(void);
    void stop(void);

    void update(void);

private:
    Ui::RS232_widget *ui;

    Generator_Curve *generator = nullptr;
    QTimer *timer = nullptr;

    QByteArray send_data;

    void init(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // RS232_WIDGET_HPP
