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
#ifndef IPV4_WO_PORT_HPP
#define IPV4_WO_PORT_HPP
//--------------------------------------------------------------------------------
#include <QPointer>
#include <QEvent>
#include <QUrl>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#define DEFAULT_IPV4_PORT   80
//--------------------------------------------------------------------------------
class IPV4_wo_port : public MyWidget
{
    Q_OBJECT

signals:
    void editingFinished(void);
    void s_get_url(QUrl);

public:
    explicit IPV4_wo_port(QWidget *parent = nullptr);
    virtual ~IPV4_wo_port();

    void setObjectName(const QString &name);

    QUrl get_url(void);
    void set_url(QUrl url);

    void set_a(int value);
    void set_b(int value);
    void set_c(int value);
    void set_d(int value);

    int get_a(void);
    int get_b(void);
    int get_c(void);
    int get_d(void);

    void setReadOnly(bool state);

    bool no_exit(void);

private slots:
    void work(void);

private:
    QPointer<QSpinBox> a;
    QPointer<QSpinBox> b;
    QPointer<QSpinBox> c;
    QPointer<QSpinBox> d;

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);

protected:
    bool eventFilter(QObject*, QEvent* event);
};
//--------------------------------------------------------------------------------
#endif // IPV4_WO_PORT_HPP
