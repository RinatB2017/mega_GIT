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
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
#include <QAbstractSocket>
//--------------------------------------------------------------------------------
class QStandardItemModel;
class MySplashScreen;
class QToolButton;
class QComboBox;
class QToolBar;
class QTcpSocket;
class RTSP_widget;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    ~MainBox();

private slots:
    void scan(void);

    void f_connect(void);
    void f_disconnect(void);

    void slotReadyRead   (void);
    void slotError       (QAbstractSocket::SocketError);
    void slotSendToServer(void);
    void slotConnected   (void);

private:
    MySplashScreen *splash;
    Ui::MainBox *ui;

    QString strHost;
    int nPort = 0;
    QTcpSocket* m_pTcpSocket;

    QStandardItemModel *model;

#ifdef USE_RTSP
    RTSP_widget *rtsp_widget = nullptr;
#endif

    void init(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
