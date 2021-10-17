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
#include "showjson.hpp"
#include "ui_showjson.h"
//--------------------------------------------------------------------------------
ShowJSON::ShowJSON(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::ShowJSON)
{
    init();
}
//--------------------------------------------------------------------------------
ShowJSON::~ShowJSON()
{
    if(model)
    {
        model->deleteLater();
    }

    delete ui;
}
//--------------------------------------------------------------------------------
void ShowJSON::init(void)
{
    ui->setupUi(this);

    model = new QJsonModel;
    ui->view->setModel(model);
}
//--------------------------------------------------------------------------------
bool ShowJSON::loadJson(QByteArray data)
{
    emit trace(Q_FUNC_INFO);

    bool ok = model->loadJson(data);
    if(ok)
    {
        ui->view->setModel(model);
        return true;
    }
    else
    {
        emit error("loadJson return false");
    }
    return false;
}
//--------------------------------------------------------------------------------
void ShowJSON::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool ShowJSON::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void ShowJSON::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void ShowJSON::save_setting(void)
{

}
//--------------------------------------------------------------------------------
