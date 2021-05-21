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
#ifndef IPV6_HPP
#define IPV6_HPP
//--------------------------------------------------------------------------------
#include <QPointer>
#include <QUrl>
//--------------------------------------------------------------------------------
#include "hexspinbox.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class IPV6 : public MyWidget
{
    Q_OBJECT

signals:
    void s_get_url(QUrl);

public:
    explicit IPV6(QWidget *parent = nullptr);
    virtual ~IPV6();

    void setObjectName(const QString &name);

    QUrl get_url(void);
    void set_url(QUrl url);

    bool no_exit(void);

private slots:
    void work(void);

private:
    QPointer<HexSpinBox> a;
    QPointer<HexSpinBox> b;
    QPointer<HexSpinBox> c;
    QPointer<HexSpinBox> d;
    QPointer<HexSpinBox> e;
    QPointer<HexSpinBox> f;

    QSpinBox *port;

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // IPV4_HPP
