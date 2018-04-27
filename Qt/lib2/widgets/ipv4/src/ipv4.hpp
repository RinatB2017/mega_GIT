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
#ifndef IPV4_HPP
#define IPV4_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
class IPV4 : public QWidget
{
    Q_OBJECT

public:
    explicit IPV4(QWidget *parent = 0);
    ~IPV4();\

    QUrl get_url(void);
    void set_url(QUrl url);

private:
    QSpinBox *a = 0;
    QSpinBox *b = 0;
    QSpinBox *c = 0;
    QSpinBox *d = 0;

    QSpinBox *port = 0;

    QSettings *settings = 0;

    void load_QSpinBox(QString group_name);
    void save_QSpinBox(QString group_name);
};
//--------------------------------------------------------------------------------
#endif // IPV4_HPP
