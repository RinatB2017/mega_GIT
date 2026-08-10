//--------------------------------------------------------------------------------
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QPushButton>
#include <QToolButton>
#include <QObject>
#include <QWidget>
#include <QIcon>
//--------------------------------------------------------------------------------
#include "treewidget.hpp"
//--------------------------------------------------------------------------------
TreeWidget::TreeWidget(QWidget *parent)
{
    QIcon icon(QLatin1String(":/mainwindow/computer.png"));

    setWindowIcon(icon);
    setWindowTitle(tr("Objects"));
    setHeaderLabel(tr("Tree"));

    QTreeWidgetItem *item = new QTreeWidgetItem(this, QStringList() << parent->metaObject()->className());
    test(item, parent);

    setMinimumSize(640, 480);
}
//--------------------------------------------------------------------------------
void TreeWidget::test(QTreeWidgetItem *tree, QObject *object)
{
    QObjectList ol = object->children();
    foreach (QObject *obj, ol)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(tree, QStringList() << obj->metaObject()->className());
        info_QPushButton(item, obj);
        info_QToolButton(item, obj);
        test(item, obj);
    }
}
//--------------------------------------------------------------------------------
void TreeWidget::info_QToolButton(QTreeWidgetItem *item, QObject *btn)
{
    QString temp;

    if(btn->metaObject()->className() != QString("QToolButton")) return;

    temp = QString("%1 [%2]")
            .arg(((QToolButton *)btn)->objectName())
            .arg(((QToolButton *)btn)->text());
    new QTreeWidgetItem(item, QStringList() << temp);
}
//--------------------------------------------------------------------------------
void TreeWidget::info_QPushButton(QTreeWidgetItem *item, QObject *btn)
{
    QString temp;

    if(btn->metaObject()->className() != QString("QPushButton")) return;

    temp = QString("%1 [%2]")
            .arg(((QPushButton *)btn)->objectName())
            .arg(((QPushButton *)btn)->text());
    new QTreeWidgetItem(item, QStringList() << temp);
}
//--------------------------------------------------------------------------------
