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
#ifndef COLLAPSIBLEWIDGET_HPP
#define COLLAPSIBLEWIDGET_HPP
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class CollapsibleWidget;
}
//--------------------------------------------------------------------------------
class CollapsibleWidget : public MyWidget
{
    Q_OBJECT

public:
    explicit CollapsibleWidget(QWidget *parent = nullptr);
    ~CollapsibleWidget();

private:
    Ui::CollapsibleWidget *ui;
    QPointer<QTimer> timer;

    enum {
        COLLAPSE_NONE = 100,
        COLLAPSE_ON = 200,
        COLLAPSE_OFF = 300
    };
    int state = COLLAPSE_NONE;

    int height_frame = 0;
    int inc_height = 0;
    int max_time_msec = 0;
    qreal max_size = 0;

    void init(void);
    void init_timer(void);
    void update(void);
    void collapse(bool new_state);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // COLLAPSIBLEWIDGET_HPP
