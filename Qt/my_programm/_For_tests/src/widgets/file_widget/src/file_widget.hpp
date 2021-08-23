/*********************************************************************************
**                                                                              **
**     Copyright (C) 2021                                                       **
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
#ifndef FILE_WIDGET_HPP
#define FILE_WIDGET_HPP
//--------------------------------------------------------------------------------
#include <QFileSystemModel>
#include <QPointer>
//--------------------------------------------------------------------------------
#include "default_widget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class File_widget;
}
//--------------------------------------------------------------------------------
class File_widget : public Default_widget
{
    Q_OBJECT

public:
    explicit File_widget(QWidget *parent = nullptr);
    virtual ~File_widget();

private:
    Ui::File_widget *ui;
    QPointer<QFileSystemModel> file_model;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // FILE_WIDGET_HPP
