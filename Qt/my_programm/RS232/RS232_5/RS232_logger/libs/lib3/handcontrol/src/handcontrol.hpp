/*********************************************************************************
**                                                                              **
**     Copyright (C) 2022                                                       **
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
#ifndef HANDCONTROL_HPP
#define HANDCONTROL_HPP
//--------------------------------------------------------------------------------
#include <QDialog>
//--------------------------------------------------------------------------------
#if QT_VERSION < QT_VERSION_CHECK(5,4,0)
#   define nullptr NULL
#endif
//--------------------------------------------------------------------------------
namespace Ui {
    class HandControl;
}
//--------------------------------------------------------------------------------
class Calibrator;
class Multimeter_V786_2;
//--------------------------------------------------------------------------------
class HandControl : public QDialog
{
    Q_OBJECT

public:
    explicit HandControl(Calibrator *calibrator,
                         Multimeter_V786_2 *multimeter,
                         QWidget *parent = nullptr);
    virtual ~HandControl();

    void set_serno(unsigned int value);
    void set_year(unsigned int value);
    void set_address(unsigned int value);
    void set_channel(unsigned int value);
    void set_timeout(unsigned int value);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

private slots:
    void send_cmd_to_multimeter(void);
    void change_cmd_multimeter(int index);

private:
    Ui::HandControl *ui;
    Calibrator *calibrator;
    Multimeter_V786_2 *multimeter;

    unsigned int serno = 0;
    unsigned int year = 0;
    unsigned int address = 0;
    unsigned int channel = 0;
    unsigned int timeout = 0;

    void init(void);
    void connect_log(void);

    void controls_visible(bool U,
                          bool I,
                          bool R,
                          bool freq,
                          bool auto_limit);
};

#endif // HANDCONTROL_HPP
//--------------------------------------------------------------------------------
