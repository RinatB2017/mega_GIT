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
#ifndef AUTOCLICKER_HPP
#define AUTOCLICKER_HPP
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class AutoClicker;
}
//--------------------------------------------------------------------------------
class AutoClicker : public MyWidget
{
    Q_OBJECT

public:
    explicit AutoClicker(QWidget *parent = nullptr);
    ~AutoClicker();

    bool test_command(const QString &command_str);

private:
    enum STATE {
        STATE_IDLE = 0,
        STATE_CMD,
        STATE_PAUSE
    };
    int current_state = STATE_IDLE;

    QPointer<QStringListModel> model;

    Ui::AutoClicker *ui;

    QPointer<QTimer> timer_autoclick;
    int current_command = 0;
    int pause_counter = 0;

    struct COMMAND {
        QString cmd;
        int cnt_param;  // wo cmd
    };

    QList<COMMAND> correct_commands;

    void init(void);
    void timer_start(void);
    void timer_stop(void);
    void test(void);
    void update(void);

    void load(void);
    void save(void);

    bool load_from_file(const QString &filename);
    bool save_to_file(const QString &filename);

    void append(void);
    void clear(void);

    bool test_commands(void);
    void run_command(const QString &data);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // AUTOCLICKER_HPP
