//--------------------------------------------------------------------------------
#ifndef TREEWIDGET_HPP
#define TREEWIDGET_HPP
//--------------------------------------------------------------------------------
#include <QTreeWidget>
//--------------------------------------------------------------------------------
class QTreeWidgetItem;
class QWidget;
class QObject;
//--------------------------------------------------------------------------------
class TreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    explicit TreeWidget(QWidget *parent);

private slots:
    void test(QTreeWidgetItem *tree, QObject *object);

private:
    void info_QPushButton(QTreeWidgetItem *item, QObject *btn);
    void info_QToolButton(QTreeWidgetItem *item, QObject *btn);
};
//--------------------------------------------------------------------------------
#endif
