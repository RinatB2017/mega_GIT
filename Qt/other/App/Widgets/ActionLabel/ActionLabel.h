#ifndef ACTIONLABEL_H
#define ACTIONLABEL_H

#include <QLabel>

class ActionLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(bool mouseover READ mouseOver)
            // в stylesheet при hover не работает text-decoration
            // поэтому заменено на динамическое свойство + style().polish
            // при enter и leave событиях
    bool MouseOver;
public:
    explicit ActionLabel (QWidget *parent = 0);
    void enterEvent (QEvent*);
    void leaveEvent (QEvent*);
    void mousePressEvent (QMouseEvent* event);
    bool mouseOver ();

signals:
    void clicked();

public slots:

};

#endif // ACTIONLABEL_H
