/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#ifndef SCAN_TOOLBAR_HPP
#define SCAN_TOOLBAR_HPP
//--------------------------------------------------------------------------------
#include <QToolBar>
//--------------------------------------------------------------------------------
class QPushButton;
class MainWindow;
class QSpinBox;
class QTimer;
//--------------------------------------------------------------------------------
class Scan_ToolBar : public QToolBar
{
    Q_OBJECT

public:
    explicit Scan_ToolBar(QWidget *parent);
    ~Scan_ToolBar();

private slots:
    void test(void);

    void start(void);
    void stop(void);

    void update(void);

private:
    MainWindow *mw = 0;

    int curr_freq = 0;

    QTimer *timer = 0;

    QSpinBox *sb_begin_freq = 0;
    QSpinBox *sb_end_freq = 0;
    QSpinBox *sb_step_freq = 0;
    QSpinBox *sb_interval_timer = 0;

    QPushButton *btn_scan_start = 0;
    QPushButton *btn_scan_stop = 0;
    QPushButton *btn_scan_test = 0;

};
//--------------------------------------------------------------------------------
#endif // SCAN_TOOLBAR_HPP
