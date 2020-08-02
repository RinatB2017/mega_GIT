/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#ifndef PTZ_DIALOG_HPP
#define PTZ_DIALOG_HPP
//--------------------------------------------------------------------------------
#include <QDialog>
#include <QUrl>
//--------------------------------------------------------------------------------
namespace Ui {
    class PTZ_dialog;
}
//--------------------------------------------------------------------------------
class PTZ_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit PTZ_dialog(QWidget *parent = nullptr);
    ~PTZ_dialog();

    void set_login(const QString &login);
    void set_password(const QString &password);
    void set_IP(QUrl url);
    void set_port(int port);
    void set_param(const QString &param);

    QString get_login(void);
    QString get_password(void);
    QUrl get_IP(void);
    int get_port(void);
    QString get_param(void);

private:
    Ui::PTZ_dialog *ui;
    QUrl url;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // PTZ_DIALOG_HPP
