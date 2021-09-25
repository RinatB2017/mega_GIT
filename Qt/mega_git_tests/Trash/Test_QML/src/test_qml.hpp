#ifndef PIXELGRID_H
#define PIXELGRID_H

#include <QQuickItem>
#include <QSGSimpleRectNode>
#include <QColor>

class Pixel final: public QSGSimpleRectNode
{
public:
    Pixel(const QRectF& rect, const QColor& color);
    virtual ~Pixel();
private:
    QSGSimpleRectNode *border;
};

class PixelGrid : public QQuickItem
{
    Q_OBJECT
public:
    PixelGrid();
protected:
    QSGNode* updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;
};

#endif // PIXELGRID_H
