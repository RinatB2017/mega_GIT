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
#ifndef TEST_WIDGET_HPP
#define TEST_WIDGET_HPP
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class TestWidget;
}
//--------------------------------------------------------------------------------
class TestWidget : public MyWidget
{
    Q_OBJECT

public:
    explicit TestWidget(QWidget *parent = nullptr);
    virtual ~TestWidget();

public slots:
    void choice_test(void);
    bool test(void);

private:
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (TestWidget::*func)(void);
    } CMD_t;
    QList<CMD> commands;

    Ui::TestWidget *ui;
    QPointer<QComboBox> cb_test;

    void createTestBar(void);
    QToolButton *add_button(QToolBar *tool_bar,
                            QToolButton *tool_button,
                            QIcon icon,
                            const QString &text,
                            const QString &tool_tip);

    void init(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // TEST_WIDGET_HPP
