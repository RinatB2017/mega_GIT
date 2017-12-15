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
#ifndef WEBSETTINGBOX_H
#define WEBSETTINGBOX_H
//--------------------------------------------------------------------------------
#include <QWebSettings>
#include <QCheckBox>
#include <QDialog>
//--------------------------------------------------------------------------------
#include "webview.hpp"
//--------------------------------------------------------------------------------
class WebSettingBox : public QDialog
{
    Q_OBJECT
public:
    WebSettingBox(QVector<ATTR> &attr,
                  QWidget *parent = 0);
    QVector<ATTR> get_attributes(void);
    
signals:
    
private slots:
    void set_default(void);
    void select_all(void);
    void clear_all(void);

private:
    QVector<ATTR> attributes;
    void init(void);

private slots:
    void accept(void);
};
//--------------------------------------------------------------------------------
#endif // WEBSETTINGBOX_H
