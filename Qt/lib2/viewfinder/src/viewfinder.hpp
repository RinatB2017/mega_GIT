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
#ifndef VIEWFINDER_HPP
#define VIEWFINDER_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
class ViewFinder : public QWidget
{
    Q_OBJECT

public:
    explicit ViewFinder(QWidget *parent = nullptr);
    virtual ~ViewFinder();

    void install(QWidget* widget);
    void remove(void);

    void test(void);

protected:
    bool eventFilter(QObject*, QEvent* event);
    void paintEvent(QPaintEvent *);

private:

};
//--------------------------------------------------------------------------------
#endif
