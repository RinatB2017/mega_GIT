/**********************************************************************************
 **                   Author: Bikbao Rinat Zinorovich                            **
 **********************************************************************************/
#include "item_widget.hpp"
//*********************************************************************************
Item_widget::Item_widget(QWidget *parent) :
    QLabel(parent)
{
    init();
}
//*********************************************************************************
void Item_widget::init(void)
{
    setFixedSize(ITEM_SIZE, ITEM_SIZE);
    setAcceptDrops(true);

    default_pixmap = QPixmap(DEFAULT_PIXMAP);
    item_clear();
}
//*********************************************************************************
Item_widget::~Item_widget()
{

}
//*********************************************************************************
void Item_widget::item_clear(void)
{
    cnt = 0;
    pixmap_name = DEFAULT_NAME;
    pixmap = QPixmap(pixmap_name);
    setPixmap(pixmap);
}
//*********************************************************************************
void Item_widget::set_pixmap(const QString &pix_name)
{
    Q_ASSERT(!pix_name.isEmpty());

    pixmap = QPixmap(pix_name);
    pixmap_name = pix_name;

    setPixmap(pixmap);
}
//*********************************************************************************
QString Item_widget::get_name(void)
{
    return pixmap_name;
}
//*********************************************************************************
void Item_widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
//        pos_x = event->pos().x();
//        pos_y = event->pos().y();

//        int x = event->mimeData()->property("pos_x").toInt();
//        int y = event->mimeData()->property("pos_y").toInt();
//        int id = 0; //TODO надо сделать
//        emit item_drag(x, y, id);

        setCursor(Qt::ClosedHandCursor);

        pixmap = QPixmap(pixmap_name);
        cnt = 1;
    }

    if(event->button() == Qt::RightButton)
    {
        if(get_name() == ":/images/apple.png") //TODO костыль
        {
            int row = property("pos_x").toInt();
            int col = property("pos_y").toInt();
            emit item_update(col, row);
        }
    }
}
//*********************************************************************************
void Item_widget::mouseMoveEvent(QMouseEvent *)
{
    if(flag_need_clear)
    {
        setPixmap(default_pixmap);
    }

    Q_ASSERT(!pixmap_name.isEmpty());

    QMimeData *mime = new QMimeData();
    mime->setProperty("pixmap_name",    pixmap_name);
    mime->setProperty("cnt",            cnt);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mime);

    drag->setPixmap(pixmap);
    drag->setHotSpot(QPoint(pos_x,
                            pos_y));

    drag->exec();
}
//*********************************************************************************
void Item_widget::mouseReleaseEvent(QMouseEvent *)
{
    setCursor(Qt::OpenHandCursor);
}
//*********************************************************************************
void Item_widget::dragEnterEvent(QDragEnterEvent *event)
{
    int x = property("pos_y").toInt();
    int y = property("pos_x").toInt();
    int id = 0; //TODO надо сделать

    qDebug() << "dragEnterEvent:" << x << y << id;

    emit item_drag(x, y, id);

    event->acceptProposedAction();
}
//*********************************************************************************
void Item_widget::dragLeaveEvent(QDragLeaveEvent *)
{

}
//*********************************************************************************
void Item_widget::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}
//*********************************************************************************
void Item_widget::dropEvent(QDropEvent *event)
{
    QString pix_name = event->mimeData()->property("pixmap_name").toString();
    if(pix_name == ":/images/apple.png")    //TODO костыль
    {
        pixmap_name = pix_name;
    }

    int x = property("pos_y").toInt();
    int y = property("pos_x").toInt();
    int id = 0; //TODO надо сделать
    emit item_drop(x, y, id);

    bool ok;
    int t_cnt = event->mimeData()->property("cnt").toInt(&ok);
    Q_ASSERT(ok);
    if(ok)
    {
        cnt += t_cnt;
        draw_cnt(cnt);
    }

    event->acceptProposedAction();
}
//*********************************************************************************
void Item_widget::draw_cnt(int value)
{
    Q_ASSERT(value >= 0);

    if(value > 0)
    {
        pixmap = QPixmap(pixmap_name);
        QPainter painter(&pixmap);
        QFont font = painter.font() ;
        font.setPointSize(font.pointSize() * 2);
        painter.setFont(font);
        painter.drawText(ITEM_SIZE / 2, //TODO надо корректнее
                         ITEM_SIZE / 2 + font.pointSize() / 2,
                         QString("%1").arg(value));
    }
    else
    {
        //pixmap = default_pixmap;
        pixmap_name = DEFAULT_NAME;
        pixmap = QPixmap(pixmap_name);
    }

    setPixmap(pixmap);
}
//*********************************************************************************
void Item_widget::set_flag_need_clear(bool value)
{
    flag_need_clear = value;
}
//*********************************************************************************
