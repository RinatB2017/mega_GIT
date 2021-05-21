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
#ifndef TESTDIALOG_HPP
#define TESTDIALOG_HPP
//--------------------------------------------------------------------------------
#include <QDialog>
//--------------------------------------------------------------------------------
namespace Ui {
    class TestDialog;
}
//--------------------------------------------------------------------------------
class TestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TestDialog(int max_index,
                        int max_offset,
                        QWidget *parent = nullptr);
    virtual ~TestDialog();

    int get_index(void);
    int get_offset(void);

private:
    Ui::TestDialog *ui;

    int max_index = 0;
    int max_offset = 0;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // TESTDIALOG_HPP
