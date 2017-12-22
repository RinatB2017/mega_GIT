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
struct ATTR
{
    QWebSettings::WebAttribute attribute;
    QString name;
    QString tooltip;
    QCheckBox *object;
    bool default_value;
};
//--------------------------------------------------------------------------------
class WebSettingBox : public QDialog
{
    Q_OBJECT
public:
    explicit WebSettingBox(QWidget *parent = 0);
    
signals:
    
private slots:
    void slot_checkbox(bool state);
    void set_default(void);

private:
    QVector<ATTR> attributes;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // WEBSETTINGBOX_H
