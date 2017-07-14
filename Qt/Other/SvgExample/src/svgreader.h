#ifndef SVGREADER_H
#define SVGREADER_H

#include <QList>
#include <QGraphicsRectItem>

class SvgReader
{
public:
    SvgReader();
    static QList<QGraphicsRectItem *> getElements(const QString filename);
    static QRectF getSizes(const QString filename);
};

#endif // SVGREADER_H
