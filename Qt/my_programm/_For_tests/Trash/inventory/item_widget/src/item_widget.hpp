/**********************************************************************************
 **                   Author: Bikbao Rinat Zinorovich                            **
 **********************************************************************************/
#ifndef ITEM_WIDGET_HPP
#define ITEM_WIDGET_HPP

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QMimeData>
#include <QPainter>
#include <QLabel>
#include <QDrag>

//#ifdef QT_DEBUG
#   include <QDebug>
//#endif

#include "defines.hpp"

QT_BEGIN_NAMESPACE
    class QMimeData;
QT_END_NAMESPACE

class Item_widget : public QLabel
{
    Q_OBJECT

public:
    explicit Item_widget(QWidget *parent = nullptr);
    virtual ~Item_widget();

    void set_pixmap(const QString &pix_name);
    QString get_name(void);

    void set_flag_need_clear(bool value);
    void item_clear(void);

    void draw_cnt(int value);

signals:
    void item_drag(int x, int y, int id);   //TODO надо убрать
    void item_drop(int x, int y, int id);
    void item_update(int x, int y);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *) override;
    void dropEvent(QDropEvent *event) override;

private:
    QString pixmap_name;
    QPixmap pixmap;

    QPixmap default_pixmap; // изображение пустой клетки
    bool flag_need_clear = true;

    int pos_x = ITEM_SIZE / 2;
    int pos_y = ITEM_SIZE / 2;
    int cnt   = 0;

    void init(void);
};

#endif // ITEM_WIDGET_HPP
