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
#ifndef SHOWJSON_HPP
#define SHOWJSON_HPP
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
#include "qjsonmodel.h"
//--------------------------------------------------------------------------------
namespace Ui {
    class ShowJSON;
}
//--------------------------------------------------------------------------------
class ShowJSON : public MyWidget
{
    Q_OBJECT

public:
    explicit ShowJSON(QWidget *parent = nullptr);
    virtual ~ShowJSON();

    bool loadJson(QByteArray data);

private:
    Ui::ShowJSON *ui;
    QPointer<QJsonModel> model;

    void init(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // SHOWJSON_HPP
