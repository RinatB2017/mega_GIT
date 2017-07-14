//--------------------------------------------------------------------------------
#ifndef _PROPERTY_EDITOR_H
#define _PROPERTY_EDITOR_H
//--------------------------------------------------------------------------------
#include <QTreeView>
#include <QStandardItemModel>
//--------------------------------------------------------------------------------
#include "delegates.hpp"
//--------------------------------------------------------------------------------
class PropertyEditor : public QTreeView
{
    Q_OBJECT

public:
    PropertyEditor(QWidget* = 0);
    void setValue(int, const QString&, const QString& = "");
    void addLineProperty(const QString&, const QString& = "", const QString& = "");
    void addIntProperty(const QString&, int, int, int, const QString& = "");
    void addCheckProperty(const QString&, Qt::CheckState = Qt::Unchecked, const QString& = "");
    void addComboProperty(const QString&, const QStringList&, const QString& = "", const QString& = "");
    void addGroup(const QString&, const QString& = "");
    QString getCurrentText(int, const QString& = "");

protected:
    bool edit(const QModelIndex&, EditTrigger, QEvent*);
    void mousePressEvent(QMouseEvent*);
    void keyPressEvent(QKeyEvent*);

private:
    QStandardItem *get_item_group(const QString&);
    QStandardItemModel* model;
};
//--------------------------------------------------------------------------------
#endif // _PROPERTY_EDITOR_H
