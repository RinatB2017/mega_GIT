#ifndef WIDGETSGROUP_H
#define WIDGETSGROUP_H

#include <QWidget>
#include <QVBoxLayout>

namespace Ui {
    class WidgetsGroup;
}

class WidgetsGroup : public QWidget
{
    Q_OBJECT
    QVBoxLayout* layoutContents;
    bool uiwasntbuilt;
public:
    explicit WidgetsGroup(QWidget *parent = nullptr);
    ~WidgetsGroup();

    void childEvent (QChildEvent* event);

    void addWidget(QWidget* widget);

private:
    Ui::WidgetsGroup *ui;
signals:
    void wantAddLayout(QLayout* layout);
private slots:
    void addLayout (QLayout* layout);  
public slots:
    void switchContents ();
};

#endif // WIDGETSGROUP_H
