#include <QTreeWidget>
#include <QDropEvent>
#include <QDebug>

#include "basedrag.hpp"

//Реализация класса 
BaseDrag::BaseDrag(QWidget *parent) : QTreeWidget(parent)
{ 
    setAcceptDrops(true);
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::DragDrop);
} 

//Функция сheckFormat вызывает GetFormat для получения списка 
//поддерживаемых форматов данных, и проверяет наличие каждого типа данных, 
//находящихся в списке.Если хотя бы один имеется, то возвращает true,и 
//разрешает перетаскивание.Иначе возворощает false, и запрещает 
//перетаскивание 
bool BaseDrag::checkFormat(QDropEvent *event)
{ 
    //qDebug() << "checkFormat";
    QStringList supportedFormats = GetFormat();
    foreach (QString format, supportedFormats)
    {
        if (event->mimeData()->hasFormat(format))
        {
            event->acceptProposedAction();
            return true;
        }
    }
    event->ignore();
    return false;
}

void BaseDrag::dragEnterEvent(QDragEnterEvent *event)
{ 
    checkFormat(event);
}

void BaseDrag::dragMoveEvent(QDragMoveEvent *event)
{ 
    checkFormat(event);
}

void BaseDrag::dropEvent(QDropEvent *event)
{ 
    if(checkFormat(event))
    {
        //Извлекаем по очереди все данные и вызываем функцию drop
        QStringList supportedFormats = GetFormat();
        foreach (QString format, supportedFormats)
        {
            QByteArray pieceData = event->mimeData()->data(format);
            QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
            drop(dataStream, event->pos(), format);
        }
    }
}

void BaseDrag::startDrag(Qt::DropActions supportedActions)
{
    QString signature;
    QByteArray sendData;
    QDataStream dataStream(&sendData, QIODevice::WriteOnly);
    //Нужно ли начать перетаскивание?
    if(!start(dataStream,signature))
        return ;
    if(!signature.size())
        return ;
    //Начало процесса перетаскивания
    QMimeData *mimeData = new QMimeData;
    mimeData->setData(signature, sendData);
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(QPoint(0,0));
    drag->setPixmap(pmIcon);
    drag->exec(Qt::MoveAction);
    delete drag;
    Q_UNUSED(supportedActions)
}

QStringList BaseDrag::GetFormat()
{
    return QStringList() << "*";
}
