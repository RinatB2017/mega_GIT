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
#ifndef MAINBOX_H
#define MAINBOX_H
//--------------------------------------------------------------------------------
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
    MainBox(QWidget *parent = 0);
    ~MainBox();

protected:
    void changeEvent(QEvent *e);

private slots:
    void test(void);
    void updateText(void);

private:
    Ui::MainBox *ui = 0;
    QWidget *parent = 0;
    QtEnumEditorFactory *comboBoxFactory = 0;
    QtEnumPropertyManager *enumManager = 0;
    QtTreePropertyBrowser *editor = 0;

    QtProperty *item0 = 0;
    QtProperty *device = 0;
    QtProperty *baudrate = 0;
    QtProperty *databits = 0;
    QtProperty *parity = 0;
    QtProperty *stopbits = 0;
    QtProperty *flow = 0;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_H
