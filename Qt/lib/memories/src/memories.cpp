/*********************************************************************************
**                                                                              **
**     Copyright (C) 2022                                                       **
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
#include "memories.hpp"
//--------------------------------------------------------------------------------
Memories::Memories(QWidget *parent)
    : QWidget{parent}
{

}
//--------------------------------------------------------------------------------
void Memories::read_file(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        emit error(QString("File %1 not open")
                   .arg(filename));
        return;
    }
    QTextStream in(&file);

    while(true)
    {
        QString temp = in.readLine();
        if(temp.isEmpty())
        {
            file.close();
            return;
        }
    }
    file.close();
}
//--------------------------------------------------------------------------------
void Memories::save_file(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        emit error(QString("File %1 not open")
                   .arg(filename));
        return;
    }

    QTextStream out(&file);

    QList<QWidget *> all_obj = findChildren<QWidget *>();
    foreach(QWidget *obj, all_obj)
    {
        out << obj->objectName();
    }
    file.close();
}
//--------------------------------------------------------------------------------
void Memories::show_image(QImage image)
{
    QLabel *label = new QLabel();
    label->setPixmap(QPixmap::fromImage(image));
    label->setFixedSize(image.width(),
                        image.height());
    label->show();
}
//--------------------------------------------------------------------------------
void Memories::draw_sinus(void)
{
    int w = 1920;
    int h = 700;

    QImage image = QImage(w, h, QImage::Format_ARGB32);
    image.fill(QColor(Qt::white));
    QPainter p(&image);

    qreal x1 = 0;
    qreal y1 = h / 2;

    QPen pen;
    pen.setColor(QColor(Qt::red));
    pen.setWidth(10);
    p.setPen(pen);

    //int offset = 100;
    for(int offset=40; offset<400; offset+=40)
    {
        for(int x=0; x<w; x++)
        {
            qreal y = 300.0*qSin(qreal(x + offset)*qreal(M_PI)/qreal(180.0)) + 350.0;
            p.drawLine(x1, y1, x, y);
            x1 = x;
            y1 = y;
        }
        x1 = 0;
        y1 = h / 2;
    }

    QLabel *label = new QLabel();
    label->setPixmap(QPixmap::fromImage(image));
    label->setFixedSize(image.width(),
                        image.height());
    label->show();
}
//--------------------------------------------------------------------------------
void Memories::name_filters(void)
{
    QString filename;
    QStringList filters;
    filters.append("Movie files (*.avi *.mov *.mpg)");

    MyFileDialog *dlg = new MyFileDialog("memories", "memories");
    dlg->setNameFilters(filters);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        filename = files.at(0);
        emit info(filename);
    }
    delete dlg;
}
//--------------------------------------------------------------------------------
void Memories::set_directory(void)
{
    QString filename;

    MyFileDialog *dlg = new MyFileDialog("memories", "memories");
    dlg->setOption(QFileDialog::ShowDirsOnly, true);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        filename = files.at(0);
        emit info(filename);
    }
    delete dlg;
}
//--------------------------------------------------------------------------------
void Memories::single_shot(void)
{
    QTimer::singleShot(0, [this]{
        emit info("OK");
    });
}
//--------------------------------------------------------------------------------
bool Memories::eventFilter(QObject *obj, QEvent *event)
{
    // установка
    // widget->installEventFilter(this);

    if(event->type() == QMouseEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = (QMouseEvent *) event;
        if(mouseEvent->button() == Qt::LeftButton)
        {
            emit info(obj->objectName());
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
