#ifndef MYTREEVIEW_HPP
#define MYTREEVIEW_HPP

#include <QTreeView>
#include <QTimer>

class MyTreeView: public QTreeView
{
    Q_OBJECT
public:
    MyTreeView(QWidget *parent = 0);

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);

private:
    QTimer timer;

private slots:
    void onSingleClick();

};

#endif
