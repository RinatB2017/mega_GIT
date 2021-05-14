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
#ifndef QHEXEDIT2_HPP
#define QHEXEDIT2_HPP
//--------------------------------------------------------------------------------
#include <QDragEnterEvent>
#include <QMessageBox>

#include "qhexedit.hpp"
//--------------------------------------------------------------------------------
class QHexEdit2 : public QHexEdit
{
    Q_OBJECT

public:
    explicit QHexEdit2(QWidget *parent = nullptr);

private:
    QString curFile;
    QFile file;
    bool isUntitled = true;

    void init(void);
    void loadFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
};
//--------------------------------------------------------------------------------
#endif // QHEXEDIT2_HPP
