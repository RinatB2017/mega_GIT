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
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include <QScrollArea>
#include <QPushButton>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class QtEnumPropertyManager;
class QtTreePropertyBrowser;
class QtEnumEditorFactory;
class QtProperty;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent = nullptr);
    virtual ~MainBox();

private slots:
    void test(void);

private:
    Ui::MainBox *ui;
    QtEnumEditorFactory *comboBoxFactory = nullptr;
    QtEnumPropertyManager *enumManager = nullptr;
    QtTreePropertyBrowser *editor = nullptr;

    QtProperty *item0 = nullptr;
    QtProperty *device = nullptr;
    QtProperty *baudrate = nullptr;
    QtProperty *databits = nullptr;
    QtProperty *parity = nullptr;
    QtProperty *stopbits = nullptr;
    QtProperty *flow = nullptr;

    void init(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_H
