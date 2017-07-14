/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#ifndef CREATORBOX_HPP
#define CREATORBOX_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QFile>
//--------------------------------------------------------------------------------
namespace Ui {
    class CreatorBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class QToolButton;
class QToolBar;
class QComboBox;
class QSpinBox;
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
class CreatorBox : public MyWidget
{
    Q_OBJECT

public:
    explicit CreatorBox(QWidget *parent);
    ~CreatorBox();

private slots:
    void create_dat3_file(void);

private:
    Ui::CreatorBox *ui = 0;

    void init(void);
    void init_widgets(void);

    QFile *file = 0;

protected:
    void changeEvent(QEvent *event);

};
//--------------------------------------------------------------------------------
#endif
