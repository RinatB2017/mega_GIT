#pragma once

#include <QWidget>
#include <QtWidgets>

class BtnWdg : public QWidget
{
    Q_OBJECT
public:
    explicit BtnWdg(const QString &text, QWidget *parent = nullptr);
    void setText(QString text);

private:
    QLabel *label;
    QToolButton *button;
    QHBoxLayout *vLayout;

signals:
    void btnClicked();

public slots:

};

//------------------------------------------------------------

class EditorDelegate: public QStyledItemDelegate
{
    Q_OBJECT
public:
    EditorDelegate(QObject *parent);
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const {
        Q_UNUSED(option);
        Q_UNUSED(index);
        return QSize(50,20);
    }
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    QList<QPair<QString, QString>> comboMap;

protected:
    void paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;

private slots:
    void commitAndCloseEditor();

signals:
    void editorClose(QStyledItemDelegate *item);
    void btnClicked();
    void valueChanged(int paramType, const QString &paramName, QVariant value) const;
};

//------------------------------------------------------------

class XYZ_ParamTree : public QTreeWidget
{
    Q_OBJECT
public:
    enum PARAM_TYPE {
        String,
        Integer,
        Color,
        Font,
        Boolean,
        ComboList,
        Button
    };
    XYZ_ParamTree(QWidget *parent = nullptr);
    QTreeWidgetItem *setParamTree(PARAM_TYPE paramType, QString paramName, QVariant value = "",
                                  QTreeWidgetItem *parentItem = nullptr, bool readonly = false);
    void setComboList(QString paramName, QStringList values);
    int getComboIdx(QString paramName, QString value);
    void *objRef;
    QStyledItemDelegate *delegate() {return m_delegate;}

private:
    QTreeWidgetItem *findItemInTree(QString paramName, QTreeWidgetItem *parentItem);
    QTreeWidgetItem *findItemInTree(QString paramName);

    EditorDelegate *m_delegate;

private slots:
    void dlgBtnClick();
    void clsEditor();
    void itemChanged(QTreeWidgetItem *item, int column);
    void test(XYZ_ParamTree::PARAM_TYPE paramType, const QString &paramName, QVariant value);

signals:
    void btnClicked(QString &paramName);
    void valueChanged(XYZ_ParamTree::PARAM_TYPE paramType, const QString &paramName, QVariant value);

};

Q_DECLARE_METATYPE(XYZ_ParamTree::PARAM_TYPE)
