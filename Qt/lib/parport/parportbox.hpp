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
#ifndef PARPORTBOX_HPP
#define PARPORTBOX_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QMap>
//--------------------------------------------------------------------------------
#define LED_ON  "background:green"
#define LED_OFF ""
//--------------------------------------------------------------------------------
enum
{
    BTN_0 = 0,
    BTN_1,
    BTN_2,
    BTN_3,
    BTN_4,
    BTN_5,
    BTN_6,
    BTN_7,
    BTN_INVERT_DATA,
    BTN_INVERT_CONTROL,
    BTN_CONTROL_AUTOFT,
    BTN_CONTROL_INIT,
    BTN_CONTROL_SELECT_IN,
    BTN_CONTROL_STROBE,
    BTN_STATUS_ERROR,
    BTN_STATUS_SELECT,
    BTN_STATUS_PAPER_OUT,
    BTN_STATUS_ACK,
    BTN_STATUS_BUSY
};
//--------------------------------------------------------------------------------
namespace Ui {
    class ParportBox;
}
//--------------------------------------------------------------------------------
class Parport;
class LedPanel;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QMessageBox;
class QFrame;
//--------------------------------------------------------------------------------
class ParportBox : public QWidget
{
    Q_OBJECT

public:
    explicit ParportBox(QWidget *parent = 0);
    ~ParportBox();

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

private slots:
    void pressed();
    void drawData();
    void on_powerButton_toggled(bool checked);
    void log(const QString &data);
    void updateText(void);

private:
    Ui::ParportBox *ui;
    QWidget *parent;
    QMap <QPushButton *, int> keymap;
#if LEDPANEL_ON
    LedPanel *ledPanel8;
    LedPanel *ledControlPanel8;
#endif
    QHBoxLayout *hbox;
    QVBoxLayout *vbox;
    QFrame *ledFrame;
    Parport *parport;
    QTimer *timer;
    QTimer *timer2;
    void connect_log(void);
    void message(const QString &text);
    void set_enable(bool state);
    void initKeyboard(void);
    void initWidgets(void);
    void initTimers(void);

    void connects(void);
    void set_timers(void);
    int open_parport(void);
};
//--------------------------------------------------------------------------------
#endif
