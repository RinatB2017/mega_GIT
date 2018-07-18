#include "test_qml.hpp"

#include <QRectF>
#include <QSGSimpleRectNode>

static const QString orange("orange");
static const int pixel_width(5);
static const int pixel_height(5);

Pixel::Pixel(const QRectF &rect, const QColor &color)
    : QSGSimpleRectNode(rect, color)
    , border(new QSGSimpleRectNode)
{
    setColor(Qt::black);
    border->setRect(this->rect().adjusted(1,1,-1,-1));
    border->setColor(color);
    appendChildNode(border);
}

Pixel::~Pixel()
{
    delete border;
}

PixelGrid::PixelGrid()
{
    setFlag(ItemHasContents);
}

QSGNode *PixelGrid::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
{
    QSGNode *node = oldNode;
    if (!node)
    {
        node = new QSGSimpleRectNode(boundingRect(), QColor(orange));

        for (int i = 0; i < 1000; i = i + pixel_width)
        {
            for (int j = 0; j < 1000; j = j + pixel_height)
                node->appendChildNode(new Pixel(QRectF(i,j,pixel_width,pixel_height), Qt::red));
        }
    }

    return node;
}
