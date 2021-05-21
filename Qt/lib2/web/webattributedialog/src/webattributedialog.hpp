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
#ifndef WEBATTRIBUTEDIALOG_HPP
#define WEBATTRIBUTEDIALOG_HPP
//--------------------------------------------------------------------------------
#include <QCheckBox>
#include <QDialog>
#include <QLabel>
//--------------------------------------------------------------------------------
#include <QWebEngineSettings>
//--------------------------------------------------------------------------------
#define ATTRIBUTE   "attribute"
//--------------------------------------------------------------------------------
namespace Ui {
    class WebAttributeDialog;
}
//--------------------------------------------------------------------------------
class WebAttributeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WebAttributeDialog(QWidget *parent = nullptr);
    virtual ~WebAttributeDialog();

    bool set(const QString &attribute_name, bool value);
    bool get(const QString &attribute_name);

    QWebEngineSettings::WebAttribute get_attr(const QString &attribute_name);

private:
    Ui::WebAttributeDialog *ui;
    QList<QCheckBox *> sl_checkboxes;

    typedef struct ATTR
    {
        QString attr_name;
        QWebEngineSettings::WebAttribute attr_value;
    } *ATTR_t;
    QList<ATTR> sl_attr;

    void init(void);
    void set_default(void);
};
//--------------------------------------------------------------------------------
#endif // WEBATTRIBUTEDIALOG_HPP
