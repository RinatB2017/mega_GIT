#include <QDataStream>
#include <QTreeWidget>
#include <QDropEvent>
#include <QWidget>
#include <QPoint>

class BaseDrag : public QTreeWidget
{ 
    Q_OBJECT

public: 
    BaseDrag(QWidget *parent = 0);

protected: 
    virtual bool start(QDataStream &dataStream, QString &dataType)
    {
        Q_UNUSED(dataStream)
        Q_UNUSED(dataType)
        return false;
    }
    virtual void drop(QDataStream &dataStream, QPoint pos, QString format)
    {
        Q_UNUSED(dataStream)
        Q_UNUSED(pos)
        Q_UNUSED(format)
    }
    virtual QStringList GetFormat();
    QPixmap pmIcon;

private: 
    void setDefaultParam(void);
    //Эта функция вызываеться когда пользователь начал перетаскивать данные на
    //наш виджет
    void dragEnterEvent(QDragEnterEvent *event);
    //Эта функция вызываеться во время движения мыши по нашему виджету
    void dragMoveEvent(QDragMoveEvent *event);
    //Эта функция вызываеться во время завершения drag-and-drop
    void dropEvent(QDropEvent *event);
    //Эта функция вызываеться во время начала перетаскивания
    void startDrag(Qt::DropActions supportedActions);

    bool checkFormat(QDropEvent *event);
};
