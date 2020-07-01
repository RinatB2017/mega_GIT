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
#ifndef COPYRIGHTER_EDITOR_HPP
#define COPYRIGHTER_EDITOR_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class Copyrighter_editor;
}
//--------------------------------------------------------------------------------
class Copyrighter_editor : public MyWidget
{
    Q_OBJECT

signals:
    void textChanged(void);

public:
    explicit Copyrighter_editor(QWidget *parent = nullptr);
    ~Copyrighter_editor();

    QString toPlainText(void);
    void append(const QString &text);

private:
    Ui::Copyrighter_editor *ui;

    void init(void);
    void check_it(void);
    void check(void);
    void check_orig(void);

    void show_info(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // COPYRIGHTER_EDITOR_HPP
