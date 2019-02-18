//--------------------------------------------------------------------------------
#include <QStandardItem>
#include <QMouseEvent>
#include <QModelIndex>
#include <QKeyEvent>
#include <QCursor>
#include <QtDebug>
//--------------------------------------------------------------------------------
#include "property_editor.hpp"
#include "r4funcs.hpp"
//--------------------------------------------------------------------------------
PropertyEditor::PropertyEditor(QWidget* parent /*= 0*/) :
    QTreeView(parent)
{
    setWindowTitle("Редактор свойств");
    model = new QStandardItemModel(0, 2);
    model->setHorizontalHeaderLabels(QStringList() << "Свойство" << "Значение");
    setModel(model);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setAllColumnsShowFocus(true);
    setRootIsDecorated(true);
}
//--------------------------------------------------------------------------------
void PropertyEditor::setValue(int index,
                              const QString& value,
                              const QString& group_name)
{
    QStandardItem* root_item = model->invisibleRootItem();
    if (!group_name.isEmpty())
    {
        root_item = get_item_group(group_name);
        if (!root_item) return;
    }
    QStandardItem* item = root_item->child(index, 1);
    if (!item)
    {
        item = new QStandardItem;
        root_item->setChild(index, 1, item);
    }
    item->setText(value);
}
//--------------------------------------------------------------------------------
void PropertyEditor::addGroup(const QString& group_name,
                              const QString& parent_group_name)
{
    QStandardItem* root_item = model->invisibleRootItem();
    if (!parent_group_name.isEmpty())
    {
        root_item = get_item_group(parent_group_name);
        if (!root_item) return;
    }
    int current_row = root_item->rowCount();
    root_item->setChild(current_row, 0, new QStandardItem(group_name));
    QStandardItem* item = new QStandardItem;
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    root_item->setChild(current_row, 1, item);
}
//--------------------------------------------------------------------------------
void PropertyEditor::addComboProperty(const QString& property_name, 
                                      const QStringList& values,
                                      const QString& default_value,
                                      const QString& group_name)
{
    QStandardItem* root_item = model->invisibleRootItem();
    if (!group_name.isEmpty())
    {
        root_item = get_item_group(group_name);
        if (!root_item) return;
    }
    int current_row = root_item->rowCount();
    root_item->setChild(current_row, 0, new QStandardItem(property_name));
    root_item->setChild(current_row, 1, new QStandardItem(default_value));
    ComboBoxDelegate* delegate = new ComboBoxDelegate(values);
    setItemDelegateForRow(current_row, delegate);
}
//--------------------------------------------------------------------------------
/*
Добавляем булевское свойство
*/
void PropertyEditor::addCheckProperty(const QString& property_name,
                                      Qt::CheckState check_state,
                                      const QString& group_name)
{
    QStandardItem* root_item = model->invisibleRootItem();
    if (!group_name.isEmpty())
    {
        root_item = get_item_group(group_name);
        if (!root_item) return;
    }
    int current_row = root_item->rowCount();
    root_item->setChild(current_row, 0, new QStandardItem(property_name));
    QStandardItem *item = new QStandardItem;
    item->setCheckable(true);
    item->setCheckState(check_state);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    root_item->setChild(current_row, 1, item);
}
//--------------------------------------------------------------------------------
/*
добавляем текстовое свойство
*/
void PropertyEditor::addLineProperty(const QString& property_name, 
                                     const QString& default_value,
                                     const QString& group_name)
{
    QStandardItem* root_item = model->invisibleRootItem();
    if (!group_name.isEmpty())
    {
        root_item = get_item_group(group_name);
        if (!root_item) return;
    }
    root_item->setChild(root_item->rowCount(), 0, new QStandardItem(property_name));
    root_item->setChild(root_item->rowCount() - 1, 1, new QStandardItem(default_value));
}
//--------------------------------------------------------------------------------
QString PropertyEditor::getCurrentText(int index,
                                       const QString& group)
{
    QStandardItem* root_item = model->invisibleRootItem();
    if (!group.isEmpty())
    {
        root_item = get_item_group(group);
        if (!root_item) return "";
    }
    return root_item->child(index, 1)->data(Qt::DisplayRole).toString();
}
//--------------------------------------------------------------------------------
void PropertyEditor::addIntProperty(const QString& property_name,
                                    int min,
                                    int max,
                                    int val,
                                    const QString& group_name)
{
    QStandardItem* root_item = model->invisibleRootItem();
    if (!group_name.isEmpty())
    {
        root_item = get_item_group(group_name);
        if (!root_item) return;
    }
    int current_row = root_item->rowCount();
    root_item->setChild(current_row, 0, new QStandardItem(property_name));
    if (isInRange(val, min, max))
        root_item->setChild(current_row, 1, new QStandardItem(QString::number(val)));
    else
        root_item->setChild(current_row, 1, new QStandardItem());
    setItemDelegateForRow(current_row, new IntEditDelegate(min, max));
}
//--------------------------------------------------------------------------------
QStandardItem *PropertyEditor::get_item_group(const QString& item_name)
{
    QModelIndex start = model->indexFromItem(model->item(0, 0));
    int role = Qt::DisplayRole;
    Qt::MatchFlags flags = Qt::MatchFixedString | Qt::MatchRecursive;
    QModelIndexList list = model->match(start, role, item_name, 1, flags);
    if (list.count() == 0) return nullptr;
    return model->itemFromIndex(list.at(0));
}
//--------------------------------------------------------------------------------
bool PropertyEditor::edit(const QModelIndex& index,
                          EditTrigger trigger,
                          QEvent* event)
{
    if (index.column() == 0) return false;
    return QTreeView::edit(index, trigger, event);
}
//--------------------------------------------------------------------------------
void PropertyEditor::mousePressEvent(QMouseEvent* event)
{
    QTreeView::mousePressEvent(event);
    if (event->button() == Qt::LeftButton)
    {
        QStandardItem* item = model->itemFromIndex(currentIndex());
        if (item && item->isCheckable())
        {
            if (item->checkState() == Qt::Unchecked)
                item->setCheckState(Qt::Checked);
            else
                item->setCheckState(Qt::Unchecked);
        }
    }
}
//--------------------------------------------------------------------------------
void PropertyEditor::keyPressEvent(QKeyEvent* event)
{
    QTreeView::keyPressEvent(event);
}
//--------------------------------------------------------------------------------
