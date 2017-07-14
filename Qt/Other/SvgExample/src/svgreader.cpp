#include "svgreader.h"
#include <QPen>
#include <QFile>
#include <QMessageBox>
#include <QDomDocument>
#include <QStringList>

SvgReader::SvgReader()
{

}

QList<QGraphicsRectItem *> SvgReader::getElements(const QString filename)
{
    QList<QGraphicsRectItem *> rectList;    // Объявим в стеке список прямоугольников

    QDomDocument doc;       // объект документа
    QFile file(filename);   // Открываем наш SVG-файл
    // Если он не открылся или не удалось передать содержимое в QDocDocument
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
    {
        return rectList;    // то возвратим список, но пустой
    }

    // Ищем в документе все объекты с тегом g
    QDomNodeList gList = doc.elementsByTagName("g");
    // Начинаем их перебирать
    for (int i = 0; i < gList.size(); i++)
    {
        QDomNode gNode = gList.item(i);     // Выделяем из списка ноду
        QDomElement rectangle = gNode.firstChildElement("rect");    // И ищем в ней элемент c тегом rect
        // Если полученный элементы не нулевой, то
        if (rectangle.isNull())
        {
            continue;
        } else {
            // начинаем формировать прямоугольник
            QGraphicsRectItem *rect = new QGraphicsRectItem();
            // Этот флаг делает объект перемещаемым, потребуется для проверки
            rect->setFlag(QGraphicsItem::ItemIsMovable);
            // Забираем размеры из тега rect
            QDomElement gElement = gNode.toElement();
            rect->setRect(rectangle.attribute("x").toInt(),
                          rectangle.attribute("y").toInt(),
                          rectangle.attribute("width").toInt(),
                          rectangle.attribute("height").toInt());

            /* Забираем из элемента ноды gNode параметры цветов
             * да да да... именно из gNode, а не из rectangle. Эти параметры храняться в теге g
             * */
            QColor fillColor(gElement.attribute("fill", "#ffffff"));    // цвет заливки
            fillColor.setAlphaF(gElement.attribute("fill-opacity","0").toFloat());
            rect->setBrush(QBrush(fillColor));

            // а также цвет и толщина абриса
            QColor strokeColor(gElement.attribute("stroke", "#000000"));
            strokeColor.setAlphaF(gElement.attribute("stroke-opacity").toFloat());

            rect->setPen(QPen(strokeColor,gElement.attribute("stroke-width", "0").toInt()));
            rectList.append(rect);  // добавляем прямоугольник в список
        }
    }
    file.close();
    return rectList;
}

QRectF SvgReader::getSizes(const QString filename)
{
    QDomDocument doc;       // инициализируем в стеке объект QDomDocument
    QFile file(filename);   // Открываем наш SVG-файл
    // Если он не открылся или не удалось передать содержимое в QDocDocument
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return QRectF(0,0,200,200); // то возвратим значения для сцены по умолчанию

    /* Далее забираем список элементов с тегом svg.
     * В случае, если список элементов будет не пустой,
     * то заберём размеры графической сцены.
     * */
    QDomNodeList list = doc.elementsByTagName("svg");
    if(list.length() > 0)
    {
        QDomElement svgElement = list.item(0).toElement();
        QStringList parameters = svgElement.attribute("viewBox").split(" ");
        return QRectF(parameters.at(0).toInt(),
                      parameters.at(1).toInt(),
                      parameters.at(2).toInt(),
                      parameters.at(3).toInt());
    }
    return QRectF(0,0,200,200);
}
