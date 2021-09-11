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
#include "qhexedit2.hpp"
//--------------------------------------------------------------------------------
QHexEdit2::QHexEdit2(QWidget *parent) :
    QHexEdit(parent)
{
    init();;
}
//--------------------------------------------------------------------------------
void QHexEdit2::init(void)
{
    setAcceptDrops(true);
}
//--------------------------------------------------------------------------------
void QHexEdit2::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->accept();
}
//--------------------------------------------------------------------------------
void QHexEdit2::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        QList<QUrl> urls = event->mimeData()->urls();
        QString filePath = urls.at(0).toLocalFile();
        loadFile(filePath);
        event->accept();
    }
}
//--------------------------------------------------------------------------------
void QHexEdit2::loadFile(const QString &fileName)
{
    file.setFileName(fileName);
    if (!setData(file))
    {
        QMessageBox::warning(this, tr("QHexEdit"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }
    setCurrentFile(fileName);
}
//--------------------------------------------------------------------------------
void QHexEdit2::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    isUntitled = fileName.isEmpty();
    setWindowModified(false);
    if (fileName.isEmpty())
        setWindowFilePath("QHexEdit");
    else
        setWindowFilePath(curFile + " - QHexEdit");
}
//--------------------------------------------------------------------------------
