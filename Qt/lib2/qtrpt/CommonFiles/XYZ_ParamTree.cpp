#include "XYZ_ParamTree.h"
#include "XYZ_ColorSelector.h"
#include "CommonClasses.h"

BtnWdg::BtnWdg(const QString &text, QWidget *parent) :
    QWidget(parent)
{
    label = new QLabel(text, this);
    button = new QToolButton(this);
    button->setText("...");
    QObject::connect(button, SIGNAL(clicked()), this, SIGNAL(btnClicked()));

    auto spacerLeft  = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto pLayout = new QHBoxLayout(this);
    pLayout->addWidget(label);
    pLayout->addItem(spacerLeft);
    pLayout->addWidget(button);

    pLayout->setContentsMargins(3,0,0,0);
    pLayout->setSpacing(0);
    this->setLayout(pLayout);
}

void BtnWdg::setText(QString text)
{
    label->setText(text);
}

//------------------------------------------------------------

EditorDelegate::EditorDelegate(QObject *parent) :
    QStyledItemDelegate (parent)
{
}

void EditorDelegate::commitAndCloseEditor()
{
    auto editor = qobject_cast<QWidget*>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
    emit editorClose(this);
}

QWidget* EditorDelegate::createEditor(QWidget *parent,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    if (index.column() == 1)
    {
        auto paramType = index.model()->data(index, Qt::UserRole).toInt();
        auto paramName = index.model()->data(index, Qt::UserRole+1).toString();
        auto value     = index.model()->data(index, Qt::EditRole).toString();

        switch (paramType)
        {
        case XYZ_ParamTree::PARAM_TYPE::Integer: {
            auto editor = new QSpinBox(parent);
            editor->setRange(0, 999999);
            #if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                QObject::connect(editor, QOverload<int>::of(&QSpinBox::valueChanged), [=](int i)
                {
                    emit valueChanged(paramType, paramName, i);
                });
            #else
                QObject::connect(editor, &QSpinBox::valueChanged, this, [=]()
                {
					emit valueChanged(paramType, paramName, editor->value());
                });
            #endif
            return editor;
            break;
        }
        case XYZ_ParamTree::PARAM_TYPE::Color: {
            auto editor = new XYZ_ColorSelector(parent);
            QMargins margins(1,1,1,1);
            editor->setMargins(margins);
            QObject::connect(editor->button, SIGNAL(clicked()), this, SIGNAL(btnClicked()));
            return editor;
            break;
        }
        case XYZ_ParamTree::PARAM_TYPE::Button:
        case XYZ_ParamTree::PARAM_TYPE::Font: {
            auto editor = new BtnWdg(value, parent);
            QObject::connect(editor, SIGNAL(btnClicked()), this, SIGNAL(btnClicked()));
            return editor;
        }
        case XYZ_ParamTree::PARAM_TYPE::ComboList: {
            auto editor = new QComboBox(parent);
            for (auto const &pair : comboMap)
                if (pair.first == paramName)
                    editor->addItem(pair.second);
            QObject::connect(editor, SIGNAL(activated(int)), this, SLOT(commitAndCloseEditor()));
            return editor;
        }
        default: return QStyledItemDelegate::createEditor(parent, option, index);
        }
    }

    return nullptr;
}

void EditorDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.column() == 1)
    {
        int paramType = index.model()->data(index, Qt::UserRole).toInt();
        switch (paramType)
        {
        case XYZ_ParamTree::PARAM_TYPE::Color: {
            auto value = index.model()->data(index, Qt::EditRole).toString();
            auto ed = qobject_cast<XYZ_ColorSelector*>(editor);
            ed->setBackGroundColor(value);
            break;
        }
        case XYZ_ParamTree::PARAM_TYPE::Button:
        case XYZ_ParamTree::PARAM_TYPE::Font: {
            auto value = index.model()->data(index, Qt::EditRole).toString();
            auto ed = qobject_cast<BtnWdg*>(editor);
            ed->setText(value);
            break;
        }
        case XYZ_ParamTree::PARAM_TYPE::ComboList: {
            auto value = index.model()->data(index, Qt::EditRole).toString();
            auto ed = qobject_cast<QComboBox*>(editor);
            ed->setCurrentIndex(ed->findText(value));
            break;
        }
        default: QStyledItemDelegate::setEditorData(editor,index);
        }
    }
    else
    {
        return;
    }
}

void EditorDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex& index) const
{
    if (index.column() == 1)
    {
        int paramType = index.model()->data(index, Qt::UserRole).toInt();
        switch (paramType)
        {
        case XYZ_ParamTree::PARAM_TYPE::Color: {
            auto ed = qobject_cast<XYZ_ColorSelector*>(editor);
            model->setData(index, ed->getBackGroundColor(), Qt::EditRole);
            QBrush brush(Qt::white);
            model->setData(index, brush, Qt::ForegroundRole);
            break;
        }
        case XYZ_ParamTree::PARAM_TYPE::Button:
        case XYZ_ParamTree::PARAM_TYPE::Font: {
            //QItemDelegate::setModelData(editor, model, index);
            break;
        }
        case XYZ_ParamTree::PARAM_TYPE::ComboList: {
            auto ed = qobject_cast<QComboBox*>(editor);
            model->setData(index, ed->currentText());
            break;
        }
        default: QStyledItemDelegate::setModelData(editor,model,index);
        }
    }

    return;
}

void EditorDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    if (index.column() == 1) {
        int paramType = index.model()->data(index, Qt::UserRole).toInt();
        switch (paramType) {
        case XYZ_ParamTree::PARAM_TYPE::Color: {
            QRect rect = option.rect;
            rect.setHeight(rect.height()-2);
            rect.setY(rect.y()+1);
            rect.setWidth(rect.width()-4);
            rect.setX(rect.x()+2);
            QString value = index.model()->data(index, Qt::EditRole).toString();
            painter->fillRect(rect, colorFromString(value));
            break;
        }
        default:
            QStyledItemDelegate::paint(painter,option,index);
        }
    }
    else
    {
        QStyledItemDelegate::paint(painter,option,index);
    }
}

//------------------------------------------------------------

XYZ_ParamTree::XYZ_ParamTree(QWidget *parent) :
    QTreeWidget(parent)
{
    qRegisterMetaType<XYZ_ParamTree::PARAM_TYPE>("TreeParams::ParamType");

    this->setColumnCount(2);
    this->setAlternatingRowColors(true);

    auto header = this->headerItem();
    header->setText(0, "Property");
    header->setText(1, "Values");

    this->setColumnWidth(0,150);
    this->setColumnWidth(1,70);
    this->setFocusPolicy(Qt::NoFocus);

    m_delegate = new EditorDelegate(this);
    this->setItemDelegate(m_delegate);

    QObject::connect(m_delegate, &QStyledItemDelegate::closeEditor, this, [=]()
    {
        clsEditor();
    });
    QObject::connect(m_delegate, SIGNAL(btnClicked()), this, SLOT(dlgBtnClick()));
    QObject::connect(m_delegate, &EditorDelegate::valueChanged, this, [=](int pType, const QString &paramName, QVariant value)
    {
        XYZ_ParamTree::PARAM_TYPE paramType = static_cast<XYZ_ParamTree::PARAM_TYPE>(pType);
        emit valueChanged(paramType, paramName, value);
    });
    QObject::connect(this, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(itemChanged(QTreeWidgetItem*,int)));

    //QObject::connect(this, SIGNAL(valueChanged(TreeParams::ParamType,const QString&,QVariant)),
    //                 this, SLOT(test(TreeParams::ParamType,const QString&,QVariant)));
}

//Sets params in the tree
QTreeWidgetItem *XYZ_ParamTree::setParamTree(PARAM_TYPE paramType, QString paramName,
                                             QVariant value, QTreeWidgetItem *parentItem, bool readonly)
{
    if (paramName.isEmpty())
        return nullptr;

    auto item = findItemInTree(paramName, parentItem);

    if (item == nullptr) {
        if (parentItem != nullptr)
            item = new QTreeWidgetItem(parentItem);
        else
            item = new QTreeWidgetItem(this);

        if (parentItem == nullptr)
            this->addTopLevelItem(item);
        else
            parentItem->addChild(item);
    }

    switch (paramType)
    {
    case String: {
        item->setText(0, paramName);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
        item->setText(1, value.toString());
        break;
    }
    case Integer: {
        item->setText(0, paramName);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
        item->setText(1, value.toString());
        break;
    }
    case Color: {
        item->setText(0, paramName);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
        item->setText(1, colorToString(value.toString()));
        break;
    }
    case Font: {
        QFont font = value.value<QFont>();
        item->setText(0, paramName);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
        item->setText(1, font.family());
        break;
    }
    case Button: {
        item->setText(0, paramName);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
        item->setText(1, value.toString());
        break;
    }
    case Boolean: {
        item->setText(0, paramName);
        value.toBool() == true ? item->setCheckState(1, Qt::Checked) : item->setCheckState(1,Qt::Unchecked);
        break;
    }
    case ComboList: {
        QVariant val = value;
        if (val.userType() == QMetaType::Int) {
            int idx = -1;
            for (auto &pair : m_delegate->comboMap)
                if (pair.first == paramName) {
                    idx++;
                    if (idx == value.toInt())
                        val = pair.second;
                }
        }

        item->setText(0, paramName);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
        item->setText(1, val.toString());
        break;
    }
    default: {

    }
    }

    item->setData(1, Qt::UserRole,   paramType);
    item->setData(1, Qt::UserRole+1, paramName);
    item->setData(1, Qt::UserRole+2, value);

    if (readonly)
    {
        Qt::ItemFlags flags = item->flags();
        flags &= ~Qt::ItemIsEditable;
        item->setFlags(flags);
        item->setBackground(0, QBrush(Qt::red, Qt::Dense6Pattern));
    }

//    if (parentItem != nullptr)
//    {
//        Qt::ItemFlags flags = parentItem->flags();
//        item->setFlags(flags);
//        item->setBackground(0, parentItem->background(0));
//    }

    return item;
}

void XYZ_ParamTree::setComboList(QString paramName, QStringList values)
{
    for (auto &str : values)
    {
        QPair<QString, QString> pair;
        pair.first = paramName;
        pair.second = str;

        m_delegate->comboMap.removeAll(pair);
        m_delegate->comboMap << pair;
    }
}

int XYZ_ParamTree::getComboIdx(QString paramName, QString value)
{
    int idx = -1;
    for (auto &pair : m_delegate->comboMap)
    {
        if (pair.first == paramName) {
            idx++;
            if (pair.second == value)
                return idx;
        }
    }
    return idx;
}

QTreeWidgetItem *XYZ_ParamTree::findItemInTree(QString paramName, QTreeWidgetItem *parentItem)
{
    if (parentItem == nullptr)
        return findItemInTree(paramName);

    QTreeWidgetItemIterator it(this);
    while (*it) {
        QTreeWidgetItem *item = (*it);
        if (item->data(1, Qt::UserRole+1) == paramName && item->parent() == parentItem)
            return item;
        ++it;
    }
    return nullptr;
}

QTreeWidgetItem *XYZ_ParamTree::findItemInTree(QString paramName)
{
    QTreeWidgetItemIterator it(this);
    while (*it) {
        QTreeWidgetItem *item = (*it);
        if (item->data(1, Qt::UserRole+1) == paramName)
            return item;
        ++it;
    }
    return nullptr;
}

//Select color from dialog and set param
void XYZ_ParamTree::dlgBtnClick()
{
    PARAM_TYPE paramType = (PARAM_TYPE)this->currentItem()->data(1, Qt::UserRole).toInt();
    QString paramName = this->currentItem()->data(1, Qt::UserRole+1).toString();

    if (paramType == XYZ_ParamTree::PARAM_TYPE::Color)
    {
        QColor color = colorFromString(this->currentItem()->text(1));

        QScopedPointer<QColorDialog> dlg(new QColorDialog(color, this));
        if (dlg->exec() == QDialog::Accepted)
        {
            color = dlg->selectedColor();
            setParamTree(paramType, paramName, color, this->currentItem()->parent());
            this->viewport()->repaint();
            emit valueChanged(paramType, paramName, color);
        };
    }
    else if (paramType == XYZ_ParamTree::PARAM_TYPE::Font)
    {
        auto font = this->currentItem()->data(1, Qt::UserRole+2).value<QFont>();
        QScopedPointer<QFontDialog> dlg(new QFontDialog(font, this));
        if (dlg->exec() == QDialog::Accepted)
        {
            QFont font = dlg->selectedFont();
            setParamTree(paramType, paramName, font, this->currentItem()->parent());
            this->viewport()->repaint();
            emit valueChanged(paramType, paramName, font);
        }
    }

    emit this->btnClicked(paramName);
}

//Change param in paramTree
void XYZ_ParamTree::clsEditor()
{
    qDebug() << "Editor CLOSED";
    auto paramType  = (PARAM_TYPE)this->currentItem()->data(1, Qt::UserRole).toInt();
    auto paramName  = this->currentItem()->data(1, Qt::UserRole+1).toString();
    auto paramValue = this->currentItem()->data(1, Qt::EditRole).toString();

    if (paramType == XYZ_ParamTree::PARAM_TYPE::Integer)
        emit valueChanged(paramType, paramName, paramValue);

    if (paramType == XYZ_ParamTree::PARAM_TYPE::String)
        emit valueChanged(paramType, paramName, paramValue);

    if (paramType == XYZ_ParamTree::PARAM_TYPE::ComboList)
        emit valueChanged(paramType, paramName, paramValue);
}

//Process click on CheckBox
void XYZ_ParamTree::itemChanged(QTreeWidgetItem *item, int column)
{
    if (column == 1 )
    {
        auto paramType   = (PARAM_TYPE)item->data(1, Qt::UserRole).toInt();
        auto paramName   = item->data(1, Qt::UserRole+1).toString();
        auto paramValue  = item->data(1, Qt::EditRole).toString();

        if (paramType == XYZ_ParamTree::PARAM_TYPE::Boolean)
        {
            QVariant v = item->checkState(1) == Qt::Checked ? true : false;
            emit valueChanged(paramType, paramName, v);
        }
    }
}

void XYZ_ParamTree::test(XYZ_ParamTree::PARAM_TYPE paramType, const QString &paramName, QVariant value)
{
    qDebug() << "Value Changed: " << paramType << paramName << value;
}
