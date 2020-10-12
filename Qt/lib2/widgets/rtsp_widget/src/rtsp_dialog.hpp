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
#ifndef RTSP_DIALOG_HPP
#define RTSP_DIALOG_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
namespace Ui {
    class RTSP_dialog;
}
//--------------------------------------------------------------------------------
class RTSP_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit RTSP_dialog(QWidget *parent = nullptr);
    ~RTSP_dialog();

    void set_login(const QString &login);
    void set_password(const QString &password);

    void set_url(QUrl url);
    QString get_address(void);

private:
    Ui::RTSP_dialog *ui;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // RTSP_DIALOG_HPP
