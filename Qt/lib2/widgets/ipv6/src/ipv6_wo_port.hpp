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
#ifndef IPV6_WO_PORT_HPP
#define IPV6_WO_PORT_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "hexspinbox.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#define DEFAULT_IPV6_PORT   80
//--------------------------------------------------------------------------------
class IPV6_wo_port : public MyWidget
{
    Q_OBJECT

signals:
    void s_get_url(QUrl);

public:
    explicit IPV6_wo_port(QWidget *parent = nullptr);
    ~IPV6_wo_port();

    void setObjectName(const QString &name);

    QUrl get_url(void);
    void set_url(QUrl url);

    bool no_exit(void);

private slots:
    void work(void);

private:
    HexSpinBox *a;
    HexSpinBox *b;
    HexSpinBox *c;
    HexSpinBox *d;
    HexSpinBox *e;
    HexSpinBox *f;

    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif // IPV4_WO_PORT_HPP
