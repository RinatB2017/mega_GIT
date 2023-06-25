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
#ifndef DOUBLEHEXEDITOR_HPP
#define DOUBLEHEXEDITOR_HPP
//--------------------------------------------------------------------------------
#include <QFrame>
//--------------------------------------------------------------------------------
class QHexEdit;
//--------------------------------------------------------------------------------
class DoubleHexEditor : public QFrame
{
    Q_OBJECT
public:
    DoubleHexEditor(const QString &caption1,
                    const QString &caption2,
                    QWidget *parent = nullptr);

    void append(const QByteArray &array1,
                const QByteArray &array2);

signals:

public slots:

private:
    QHexEdit *hexedit1;
    QHexEdit *hexedit2;

};
//--------------------------------------------------------------------------------
#endif // DOUBLEHEXEDITOR_HPP
