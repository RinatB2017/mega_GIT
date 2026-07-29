/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#ifndef NOTEBOOK_HPP
#define NOTEBOOK_HPP
//--------------------------------------------------------------------------------
#include <QStackedWidget>
#include <QApplication>
#include <QPushButton>
#include <QTextEdit>
#include <QFrame>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class Notebook : public MyWidget
{
    Q_OBJECT
public:
    explicit Notebook(QWidget *parent = nullptr);
    virtual ~Notebook();

    void check_modified();

public slots:
    void btn_click();
    void save_all();

private:
    void init();

    bool load_data(QString name, QStringList *data);
    bool save_data(QString name, QStringList *data);

    QStringList alphabet;
    QStackedWidget *sw = nullptr;
    QPushButton *btn_save_all = nullptr;

    void updateText();
    bool programm_is_exit();
    void load_setting();
    void save_setting();
};
//--------------------------------------------------------------------------------
#endif
