#ifndef ACTIONLABEL_H
#define ACTIONLABEL_H

#include <QLabel>

class ActionLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(bool mouseover READ mouseOver)
            // � stylesheet ��� hover �� �������� text-decoration
            // ������� �������� �� ������������ �������� + style().polish
            // ��� enter � leave ��������
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
